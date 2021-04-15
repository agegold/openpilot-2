﻿#!/usr/bin/env python3
import unittest
import importlib
from selfdrive.car.fingerprints import all_known_cars
from selfdrive.car.car_helpers import interfaces
from selfdrive.car.fingerprints import _FINGERPRINTS as FINGERPRINTS

from cereal import car

from selfdrive.car.hyundai.values import Buttons, CarControllerParams, CAR, FEATURES
import cereal.messaging as messaging


class TestCarInterfaces(unittest.TestCase):
  def __init__(self):  
      self.sm = messaging.SubMaster(['modelV2','lateralPlan','radarState'])
      
  def test_car_interfaces(self):
    all_cars = all_known_cars()

    for car_name in  all_cars:
     
      if car_name != CAR.GRANDEUR_HEV_19:
         continue

      print(car_name) 
      fingerprint = FINGERPRINTS[car_name][0]

      CarInterface, CarController, CarState = interfaces[car_name]
      fingerprints = {
        0: fingerprint,
        1: fingerprint,
        2: fingerprint,
      }

      car_fw = []

      car_params = CarInterface.get_params(car_name, fingerprints, car_fw)
      car_interface = CarInterface(car_params, CarController, CarState)
      assert car_params
      assert car_interface

      self.assertGreater(car_params.mass, 1)
      self.assertGreater(car_params.steerRateCost, 1e-3)

      if car_params.steerControlType != car.CarParams.SteerControlType.angle:
        tuning = car_params.lateralTuning.which()
        if tuning == 'pid':
          self.assertTrue(len(car_params.lateralTuning.pid.kpV))
        elif tuning == 'lqr':
          self.assertTrue(len(car_params.lateralTuning.lqr.a))
        elif tuning == 'indi':
          self.assertTrue(len(car_params.lateralTuning.indi.outerLoopGainV))

      # Run car interface
      CC = car.CarControl.new_message()
      for _ in range(10):
        car_interface.update(CC, [])
        car_interface.apply(CC, self.sm, car_params)
        #car_interface.apply(CC, self.sm, car_params)

      CC = car.CarControl.new_message()
      CC.enabled = True
      for _ in range(10):
        car_interface.update(CC, [])
        car_interface.apply(CC, self.sm, car_params)
        #car_interface.apply(CC, self.sm, car_params)

      # Test radar interface
      RadarInterface = importlib.import_module('selfdrive.car.%s.radar_interface' % car_params.carName).RadarInterface
      radar_interface = RadarInterface(car_params)
      assert radar_interface

      # Run radar interface once
      radar_interface.update([])
      if not car_params.radarOffCan and hasattr(radar_interface, '_update') and hasattr(radar_interface, 'trigger_msg'):
        radar_interface._update([radar_interface.trigger_msg])

if __name__ == "__main__":
  #unittest.main()
  CI = TestCarInterfaces()

  CI.test_car_interfaces()

