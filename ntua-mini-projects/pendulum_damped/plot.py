import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

measurement = pd.read_csv("pendulum_input.csv")
measurement.columns =  ['time', 'theta_truth', 'omega', 'omega_truth', 'ddtheta_truth']

# Result is expected to be a 5-column dataframe.
# Feel free to adjust according to needs.
# @time     : the timestamp
# @theta    : the EKF's predicted displacement value
# @dtheta   : the EKF's predicted speed value
# @theta_cov    : the EKF's covariance for @theta
# @dtheta_cov   : the EKF's covariance for @dtheta
result = pd.read_csv("pendulum_ekf_result.csv")
result.columns = ['time', 'theta', 'dtheta', 'theta_cov', 'dtheta_cov']


square_error = measurement['theta_truth'] - result['theta']
square_error = square_error**2
print('Theta MSE: {:.10f}'.format(square_error.mean()))

square_error = measurement['omega_truth'] - result['dtheta']
square_error = square_error**2
print('Omega MSE: {:.10f}'.format(square_error.mean()))


# Show the minor grid lines with very faint and almost transparent grey lines
# plt.title("Pendulum Angle", fontsize=fontSize)

fontSize=15
plt.figure(figsize=(10.4,3.8))
plt.grid(b=True, which='major', color='#999999', linestyle='--')
plt.minorticks_on()
plt.grid(b=True, which='minor', color='#999999', linestyle='--', alpha=0.2)
plt.plot(measurement['time'], np.rad2deg(measurement['omega']), 'rx', label='Measurement')
plt.plot(result_sd['time'], np.rad2deg(result_sd['dtheta']), 'b-.', label='Standard Diff. variant')
# plt.plot(result_ad['time'], np.rad2deg(result_ad['dtheta']), 'g', label='Automatic Diff. variant')
plt.xticks(fontsize=fontSize)
plt.yticks(fontsize=fontSize)
plt.ylim(-270, 220)
# plt.xlim(-0.1, 3.2)
plt.ylabel('Angular Velocity ω (°/s)', fontsize=fontSize)
plt.xlabel('Elapsed time (s)', fontsize=fontSize)
plt.subplots_adjust(left=0.10, bottom=0.15, right=0.99, top=0.99) #bottom=0.., wspace=None, hspace=None
plt.legend(loc='lower center', fontsize=fontSize, ncol=3, borderaxespad=0.0)
# plt.legend(loc='lower right', fontsize=fontSize, prop={'size': 12})


plt.figure(figsize=(10.4,3.8))
plt.grid(b=True, which='major', color='#999999', linestyle='--')
plt.minorticks_on()
plt.grid(b=True, which='minor', color='#999999', linestyle='--', alpha=0.2)
plt.plot(measurement['time'], np.rad2deg(measurement['theta']), 'k--', label='True value')
plt.plot(result_sd['time'], np.rad2deg(result_sd['theta']), 'b-.', label='Standard Diff. variant')
# plt.plot(result_ad['time'], np.rad2deg(result_ad['theta']), 'g', label='Automatic Diff. variant')
plt.xticks(fontsize=fontSize)
plt.yticks(fontsize=fontSize)
plt.ylim(-28, 34)
# plt.xlim(-0.1, 3.2)
plt.ylabel('Oscillation angle Θ (°)', fontsize=fontSize)
plt.xlabel('Elapsed time (s)', fontsize=fontSize)
plt.subplots_adjust(left=0.08, bottom=0.15, right=0.99, top=0.99) #bottom=0.., wspace=None, hspace=None
# plt.legend(loc='upper center', fontsize=fontSize, ncol=3, borderaxespad=0.0)
plt.legend(loc='lower right', fontsize=fontSize, prop={'size': 12})

plt.show()

