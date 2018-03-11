#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid;
  PID pid2;
  // TODO: Initialize the pid variable.
  
  pid.Init(0.09, 0.00002, 0.0001);
  //pid.Init(0.12, 0.0002, 0.001);		// For speed = 15
  //pid.Init(0.2, 0.0005, 0.01);		// For speed = 10

  pid2.Init(0.16,0.0009,0.001);

  h.onMessage([&pid, &pid2](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          double throttle_value;

          double desire_speed = 15;
          

          //Adjust target speed according to steering angle
          /*
          if ((angle > 2.0) || (angle < - 2.0))
          {
          	desire_speed = 12;
          	pid.Kp = 0.2;
          	pid.Ki = 0.0005;
          	pid.Kd = 0.0001;
          }

          if ((angle <= 1.0) && (angle >= - 1.0))
          {
          	desire_speed = 30;
          	pid.Kp = 0.09;
          	pid.Ki = 0.00004;
          	pid.Kd = 0.00001;
          }
          */

          double speed_err = desire_speed - speed;

          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          pid.UpdateError(cte);
          std::cout << "============================================" << std::endl;
          std::cout << pid.Kp << std::endl;
          std::cout << pid.Ki << std::endl;
          std::cout << pid.Kd << std::endl;
          std::cout << pid.p_error << std::endl;
		  std::cout << pid.i_error << std::endl;
		  std::cout << pid.d_error << std::endl;
		  std::cout << angle << std::endl;
		  std::cout << "============================================" << std::endl;


		  steer_value = - pid.Kp * pid.p_error - pid.Kd * pid.d_error - pid.Ki * pid.i_error;

          
          if (steer_value < -1) steer_value = -1;
          if (steer_value > 1) steer_value = 1;


          /*
          * Speed PID controller
          */

          pid2.UpdateError(speed_err);
          throttle_value =  pid2.Kp * pid2.p_error + pid2.Kd * pid2.d_error + pid2.Ki * pid2.i_error;
          if (throttle_value < 0) throttle_value = 0;
          if (throttle_value > 1) throttle_value = 1;

          
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle_value;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
