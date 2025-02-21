# Cascade_PID_controller
The Cascade PID Controller is a control system architecture that enhances the performance of a traditional PID controller by incorporating a second-level controller within the feedback loop. It is primarily used in systems where two levels of control are necessary for precise regulation of output, such as temperature control in furnaces, speed control in motors, or pressure control in industrial systems.

### Installation and running details:
I'm Using Ubuntu 22.04 LTS
```bash
git https://github.com/salilnpatil/Cascade_PID_controller.git
cd Cascade_PID_controller/
sudo apt install gnuplot -y
gcc cascade_pid_controller.c -o cascade_pid_controller -lm
./cascade_pid_controller
```
### Demo:

<p align="center">

<img src="https://github.com/salilnpatil/Cascade_PID_controller/blob/main/assets/cascade-pid-controller.gif" width="400" height="250">

</p>

