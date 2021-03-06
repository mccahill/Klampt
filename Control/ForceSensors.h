#ifndef CONTROL_FORCE_SENSORS_H
#define CONTROL_FORCE_SENSORS_H

#include "Sensor.h"
#include <KrisLibrary/math3d/primitives.h>
using namespace Math3D;

/** @ingroup Control
 * @brief Simulates a contact sensor / tactile sensor.
 * 
 * The sensor comprises a block of space which needs to be placed on the
 * surface of a robot.  The x-y coordinates of this block form a 
 * "contact patch" over which the sensor obtains readings.  The sensor origin
 * is the center of this block.
 *
 * If the simulator generates any contacts inside the contact patch, where
 * the contact z-coordinate lies within patchTolerance of 0, then the sensor
 * reports contact.  The sensor can also have up to 3 axes of force sensing.
 * 
 * If falloffCoefficient=0, then the entire patch basically acts as a 
 * rigid strain gauge.  Otherwise, the contact force at local point
 * (x,y) attenuates according to the weight
 * (1-4*|x||y|/(xpatchSize*ypatchSize))^falloffCoefficient so that forces near
 * the edge of the patch are reduced in their influence on the measurement.
 */
class ContactSensor : public SensorBase
{
 public:
  ContactSensor();
  virtual const char* Type() const { return "ContactSensor"; }
  virtual void Simulate(ControlledRobotSimulator* robot,WorldSimulation* sim);
  virtual void SimulateKinematic(Robot& robot,RobotWorld& world);
  virtual void Reset();
  virtual void MeasurementNames(vector<string>& names) const;
  virtual void GetMeasurements(vector<double>& values) const;
  virtual void SetMeasurements(const vector<double>& values);
  virtual map<string,string> Settings() const;
  virtual bool GetSetting(const string& name,string& str) const;
  virtual bool SetSetting(const string& name,const string& str);
  virtual void DrawGL(const Robot& robot,const vector<double>& measurements);

  int link;                ///< The link on which the sensor is located
  RigidTransform Tsensor;  ///< Local frame of the sensor (by convention, origin is at contact patch center, z is normal to surface, out of robot)
  Vector2 patchMin,patchMax;///< The 2D contact patch in the local frame of the sensor 
  Real patchTolerance;     ///< The deformation tolerance of the contact patch (default 0.001)
  bool hasForce[3];        ///< If an element is true, that component of force is measured (default false)
  Vector3 fResolution;     ///< Resolution of the sensor (default 0)
  Vector3 fVariance;       ///< Estimated variance of the sensor (default 0)
  Real fSensitivity;       ///< The minimum z force needed to register contact (default 0)
  Vector3 fSaturation;     ///< The maximum force registered on each axis (default inf)
  Real falloffCoefficient; ///< The falloff coefficient (default 0)

  bool contact;            ///< Measurement: true if contact has been made
  Vector3 force;           ///< Measurement: the force magnitude
};

/** @ingroup Control
 * @brief Simulates a force-torque sensor mounted between a link and its
 * parent. Can be configured to be up to 6DOF.
 */
class ForceTorqueSensor : public SensorBase
{
 public:
  ForceTorqueSensor();
  virtual const char* Type() const { return "ForceTorqueSensor"; }
  virtual void Simulate(ControlledRobotSimulator* robot,WorldSimulation* sim);
  virtual void SimulateKinematic(Robot& robot,RobotWorld& world);
  virtual void Reset();
  virtual void MeasurementNames(vector<string>& names) const;
  virtual void GetMeasurements(vector<double>& values) const;
  virtual void SetMeasurements(const vector<double>& values);
  virtual map<string,string> Settings() const;
  virtual bool GetSetting(const string& name,string& str) const;
  virtual bool SetSetting(const string& name,const string& str);
  virtual void DrawGL(const Robot& robot,const vector<double>& measurements);

  int link;                ///< The link on which the sensor is located (between link and parent)
  Vector3 localPos;        ///< The position of the sensor, in the local frame
  bool hasForce[3];        ///< true if force is measured along the given axis (default false)
  bool hasTorque[3];       ///< true if torque is measured along the given axis (default false)
  Vector3 fVariance, tVariance; ///< Estimated variance of the sensor (default 0)

  Vector3 f,t;             ///< Measurement: the force/torque at the given position, on the link (negative on the parent link)
};

#endif 
