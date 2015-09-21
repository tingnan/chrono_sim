#ifndef INCLUDE_CONTROLLER_H_
#define INCLUDE_CONTROLLER_H_

#include <vector>

#include <motion_functions/ChFunction_Base.h>
#include <physics/ChSystem.h>
#include <physics/ChLinkEngine.h>

class Controller {
public:
  Controller(chrono::ChSystem *ch_system, class Robot *i_robot)
      : ch_system_(ch_system), robot_(i_robot) {}
  // Step the controller
  void Step(double dt);
  // get the toruqe for joint i
  size_t GetNumEngines();
  chrono::ChLinkEngine *GetEngine(size_t i);
  double GetExtTorque(size_t i, double t);
  double GetPatternAngle(size_t i, double t);
  double GetPatternAngularSpeed(size_t i, double t);

private:
  chrono::ChSystem *ch_system_;
  class Robot *robot_;
  chrono::ChVectorDynamic<> torques_ext_;
  // Parametr for the CPG
  double omega_ = 0.2 * chrono::CH_C_2PI;
  double amplitude_ = 0.5;
};

namespace chrono {
class ChFunctionController : public ChFunction {
public:
  ChFunctionController(size_t index, Controller *controller)
      : index_(index), controller_(controller) {}
  ~ChFunctionController() {}

  ChFunction *new_Duplicate() {
    return new ChFunctionController(index_, controller_);
  }

  int Get_Type() { return 9527; }

  double Get_y(double curr_t) {
    return ComputePDTorque(curr_t) - controller_->GetExtTorque(index_, curr_t);
  }

  double Get_y_dx(double new_t) {
    // it does not make sense to provide dtau/dt
    return 0;
  }

  double p_gain = 1.0;
  double d_gain = 0.1;

protected:
  // The low level PID controller in motor.
  double ComputePDTorque(double t) {
    double desired_angle = controller_->GetPatternAngle(index_, t);
    double desired_angular_speed =
        controller_->GetPatternAngularSpeed(index_, t);
    auto engine = controller_->GetEngine(index_);
    double curr_angle = engine->Get_mot_rot();
    double curr_angular_speed = engine->Get_mot_rot_dt();
    double torque = p_gain * (desired_angle - curr_angle) +
                    d_gain * (desired_angular_speed - curr_angular_speed);
    torque = std::max(std::min(1.0, torque), -1.0);
    return torque;
  }

  Controller *controller_;
  size_t index_;
};
} // namespace chrono

#endif // INCLUDE_CHFUNCTION_CONTROLLER_H_
