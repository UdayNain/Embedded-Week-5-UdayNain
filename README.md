# Embedded-Week-5

Last week you worked on the motor chassis controls which is the bottom half of the robots. This week we will take a look at the top half (Pitch, Yaw, Shoot).

## Pitch 

Pitch refers to the up and down movement of the robot's top half. This movement is typically controlled by rotating the structure around a horizontal axis. In robotics, pitch control is crucial for tasks that require precise vertical alignment, such as adjusting the angle of a camera or arm. When you control the pitch, you're essentially changing the tilt of the robot's head or the angle of an attachment.

### Sample Implementation 

```cpp
DJIMotor yaw(5, CANHandler::CANBUS_1, GIMBLY,"Yeah");
```

##  Yaw

Yaw refers to the left and right rotation of the robot's top half around a vertical axis. This movement allows the robot to turn and face different directions without changing its location. Yaw is essential for navigation, aiming, and any situation where the robot needs to adjust its orientation relative to its surroundings.

### Sample Implementation

```cpp
DJIMotor pitch(7, CANHandler::CANBUS_2, GIMBLY,"Peach");
```

## Conclusion

Both pitch and yaw are part of the robot's degrees of freedom, which allow it to interact with its environment in more complex ways. Understanding how to control these movements is fundamental for operating the robot effectively in various tasks.

## Shooting



### Instantiating Fly Wheel and Indexer
```p
DJIMotor indexer(7, CANHandler::CANBUS_2, C610,"Indexer");
DJIMotor RFLYWHEEL(8, CANHandler::CANBUS_2, M3508,"RightFly");
DJIMotor LFLYWHEEL(5, CANHandler::CANBUS_2, M3508,"LeftFly");
```

