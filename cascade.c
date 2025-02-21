#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
int update_outer_loop = 0;
typedef struct 
{
    double kp, ki, kd;
    double prev_error;
    double integral;
} PID;

double pid_update(PID *pid, double setpoint, double measured, double dt)
{
    double error = setpoint - measured;
    pid->integral += error * dt;
    double derivative = (error - pid->prev_error) / dt;
    double output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
    pid->prev_error = error;
    return output;
}

int main()
{
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (!gnuplot)
    {
        printf("Error starting gnuplot\n");
        return -1;
    }

    // Configure GNUPlot for Live Updates
    fprintf(gnuplot, "set title 'Live Cascade PID Response'\n");
    fprintf(gnuplot, "set xlabel 'Time Steps'\n");
    fprintf(gnuplot, "set ylabel 'Position'\n");
    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "set key left top\n");
    fprintf(gnuplot, "set xrange [0:300]\n");
    fprintf(gnuplot, "set yrange [0:1.2]\n");
    fflush(gnuplot);

    PID inner_pid = {5.0, 0.05, 0.05, 0, 0}; 
    PID outer_pid = {2.0, 0.02, 0.005, 0, 0};

    
    double setpoint = 1.0;
    double measured_pos = 0;
    double measured_vel = 0;
    double dt = 0.1;
    int steps = 100000;
    double time_steps[steps + 1];
    double measured_values[steps + 1];

    for (int i = 0; i <= steps; i++) 
    {   
        double outer_control = 0.0f;
        if(update_outer_loop > 1)
        {        
            outer_control = pid_update(&outer_pid, setpoint, measured_pos, dt);
            update_outer_loop = 0;
        }
        double inner_control = pid_update(&inner_pid, outer_control, measured_vel, dt);

        measured_vel += inner_control * dt;
        measured_pos += measured_vel * dt;

        time_steps[i] = i;
        measured_values[i] = measured_pos;


        fprintf(gnuplot, "plot '-' using 1:2 with lines title 'Measured', '-' using 1:2 with lines title 'Setpoint'\n");


        for (int j = 0; j <= i; j++) {
            fprintf(gnuplot, "%lf %lf\n", time_steps[j], measured_values[j]);
        }
        fprintf(gnuplot, "e\n");

        for (int j = 0; j <= i; j++) {
            fprintf(gnuplot, "%lf %lf\n", time_steps[j], setpoint);
        }
        fprintf(gnuplot, "e\n");

        fflush(gnuplot);
        usleep(50000);
        update_outer_loop++;
    }

    pclose(gnuplot);
    return 0;
}
