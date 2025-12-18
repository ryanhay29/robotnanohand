// Copyright 2021 Codethink Ltd

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This file has been modified from the Hamsa project.

#include "hand.h"

Hand::Hand() {
    Servos = SCServo();
}

bool Hand::setup_servos() {
    if (!Servos.isConnected()) {
        std::cerr << "Error: Hand not detected!" << std::endl;
        connected = false;
        return false;
    }
    Servos.EnableTorque(1, 1);
    usleep(100 * USLEEP_MULTIPLIER);
    connected = true;
    return true;
}

int Hand::curl(float percentage, // 1 = fully extended, 0 = fully curled
               int time,
               int in,
               int out,
               int curl_id) {
    int servoPosition;
    if (out < in){
        servoPosition = (((out - in) * percentage) + in);
    }else {
        servoPosition = (in - ((in - out) * percentage));
    }
    return Servos.WritePos(curl_id,  servoPosition,  time);
}

int Hand::wiggle(float percentage, // 1 = fully right, 0 = fully left
                 int time,
                 int left,
                 int right,
                 int wiggle_id) {
    int servoPosition;
    if (left > right){
        servoPosition = (((left - right) * percentage) + right);
    }else {
        servoPosition = (right - ((right - left) * percentage));
    }
    return Servos.WritePos(wiggle_id,  servoPosition,  time);
}


float Hand::curl_position(int in, int out, int curl_id) {
    float percentage;
    float servoPosition = Servos.ReadPos(curl_id);
    if (out < in){
        percentage = (servoPosition - in)/(out - in);
    } else {
        percentage = (in - servoPosition)/(in - out);
    }
    return percentage;
}

float Hand::wiggle_position(int left, int right, int wiggle_id) {
    float percentage;
    float servoPosition = Servos.ReadPos(wiggle_id);
    if (left > right){
        percentage = (servoPosition - right)/(left - right);
    } else {
        percentage = (right - servoPosition)/(right-left);
    }
    return percentage;
}

