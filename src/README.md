#Reflections for PID control project

## Effects of the P, I and D channel




According to classic control theory, 

- P channel (proportional) added an additional DC gain component to the plant, it just reduced the steady-state error (ess) but also brought overshoot to the system. 
- I channel (integral) provided an additional pole to the original plant, that is, leveled up the system type, which gave the system ability to track more sophisticated signal. In practical use, integral channel reduced or eliminate ess. 
- D channel (derivative) was often used for predictive control, since it made use of the error's derivative to improve transient response for the system. But the D channel also amplified high frequency noise. 

## Parameters tuning 
In this project, I've manually tuning the PID hyperparameters since the mathematical model for steering control was not easy to derive. The steps I followed was listed below:
1. Choose $K_P$. The steer angle range was limited to [-1, 1], so Kp value should be small enough to ensure the stability while steering, larger Kp value may cause oscillations of the vehicle. 
2. Choose $K_I$. $K_I$ can eliminate steay-state error while larger $K_I$ may cause the system transient response more aggresive. The appropriate value of KI was chosen to balance the system robustness and dynamic performance.
3. Choose $K_D$. $K_D$ in this project was not necessary but smaller $K_D$ would be OK.
