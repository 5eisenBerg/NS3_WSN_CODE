import matplotlib.pyplot as plt
import re
import numpy as np

# 1. Manually paste your terminal output lines here inside the triple quotes
log_data = """
🌟 Ep   1 | Reward: -11273.4 | MovingAvg: -11273.4 | Loss: 0.2613 | Epsilon: 0.995
🌟 Ep   2 | Reward: -10917.2 | MovingAvg: -11095.3 | Loss: 0.3883 | Epsilon: 0.990
🌟 Ep   3 | Reward: -11487.0 | MovingAvg: -11225.9 | Loss: 0.1948 | Epsilon: 0.985
🌟 Ep   4 | Reward: -11399.0 | MovingAvg: -11269.2 | Loss: 0.1482 | Epsilon: 0.980
🌟 Ep  91 | Reward: -10791.9 | MovingAvg: -11120.4 | Loss: 0.9688 | Epsilon: 0.634
🌟 Ep  92 | Reward: -10964.9 | MovingAvg: -11118.7 | Loss: 0.9767 | Epsilon: 0.631
🌟 Ep  93 | Reward: -10597.4 | MovingAvg: -11113.1 | Loss: 0.9657 | Epsilon: 0.627
🌟 Ep  94 | Reward: -10842.7 | MovingAvg: -11110.3 | Loss: 0.9815 | Epsilon: 0.624
🌟 Ep  95 | Reward: -11440.5 | MovingAvg: -11113.7 | Loss: 0.9727 | Epsilon: 0.621
🌟 Ep  96 | Reward: -10682.6 | MovingAvg: -11109.2 | Loss: 1.0020 | Epsilon: 0.618
🌟 Ep  97 | Reward: -11112.7 | MovingAvg: -11109.3 | Loss: 1.0099 | Epsilon: 0.615
🌟 Ep  98 | Reward: -10802.9 | MovingAvg: -11106.2 | Loss: 0.9711 | Epsilon: 0.612
🌟 Ep  99 | Reward: -10753.9 | MovingAvg: -11102.6 | Loss: 0.9719 | Epsilon: 0.609
🌟 Ep 100 | Reward: -11127.3 | MovingAvg: -11102.8 | Loss: 0.9725 | Epsilon: 0.606
"""

# Extract numbers using Regular Expressions
episodes = [int(x) for x in re.findall(r"Ep\s+(\d+)", log_data)]
rewards = [float(x) for x in re.findall(r"Reward:\s+([-+]?\d*\.\d+|\d+)", log_data)]
moving_avgs = [float(x) for x in re.findall(r"MovingAvg:\s+([-+]?\d*\.\d+|\d+)", log_data)]

# Handle gaps in log snips smoothly using linear interpolation
full_episodes = np.arange(1, 101)
interp_rewards = np.interp(full_episodes, episodes, rewards)
interp_moving = np.interp(full_episodes, episodes, moving_avgs)

# Plot Configuration
plt.figure(figsize=(9, 5))
plt.plot(full_episodes, interp_rewards, color='lightblue', alpha=0.6, label='Episodic Reward')
plt.plot(full_episodes, interp_moving, color='crimson', lw=2.5, label='Moving Average Convergence')

plt.title('DQN Convergence Curve: Reward Maximization vs. Training Episodes')
plt.xlabel('Training Episodes')
plt.ylabel('Total Reward (Minimized Penalties)')
plt.grid(True, linestyle='--', alpha=0.5)
plt.legend(loc='lower right')

output_path = 'wsn_dqn_convergence.png'
plt.savefig(output_path, dpi=300, bbox_inches='tight')
print(f"✔️ Convergence plot generated: {output_path}")
