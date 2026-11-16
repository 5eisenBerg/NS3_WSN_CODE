# 📡 Project Documentation: AI-Driven WSN with Triple-Queue Priority & Dynamic Thresholding

## 📌 Project Vision
The core idea of this project is to create an **Intelligent Wireless Sensor Network (WSN)** that doesn't just blindly send data, but *learns* how to manage its resources autonomously. By using **Deep Reinforcement Learning (DQN)**, each sensor node becomes an adaptive agent capable of balancing the critical trade-off between **Data Accuracy**, **Network Latency**, and **Energy Survival**.

---

## 🛠 Software Architecture (The "Team")
Our project works by bridging two powerful worlds:
1.  **ns-3 (The Physical Simulator):** A high-fidelity network simulator that handles the physics of wireless signals, battery drainage, and node mobility.
2.  **PyTorch DQN (The Brain):** A neural network that observes the state of the nodes and makes optimal decisions on when and where to send data.
3.  **OpenAI Gym/ns3gym (The Messenger):** The interface that allows the Brain to talk to the Physical world in real-time.

---

## 🚀 Key Innovations & "What We Have Done"
We have significantly upgraded the standard WSN model with three advanced technical layers:

### 1. The Triple-Queue Priority System 🚦
Instead of one messy bucket for all data, we implemented a structured sorting engine:
*   **Priority Queue (The "Spike" Alert):** Used only when data changes suddenly (e.g., a massive jump in temperature or gas levels). This gets "VIP" transmission priority.
*   **Critical Queue:** For data that is dangerous or important (above the threshold) but steady.
*   **Normal Queue:** For routine background data that isn't urgent.

### 2. Dynamic Adaptive Thresholding 🧠
Static thresholds (like a hard-coded "50") are inefficient. We implemented logic where the threshold **adapts** based on:
*   **Environment Noise:** Using an Exponential Moving Average (EMA) to tune into the background "hum" of the sensor.
*   **Energy Levels:** As a node's battery dies, it automatically raises its threshold. This makes the node "stingy" with its remaining power, only waking up to send truly vital alerts.

### 3. Temporal Data Discarding (TTL) ⏳
Old data is often useless data. We added a **Time-To-Live (TTL)** counter to routine traffic. If a "Normal" packet sits in the queue too long without being sent, the node deletes it to save energy and bandwidth for newer, more relevant updates.

---

## 📊 Technical Deep-Dive

### The Action Space (Decisions)
The AI agent can choose between 5 distinct actions:
1.  **Send to Sink 0, 1, or 2:** Optimize routing to different base stations.
2.  **Wait / Buffer:** Hold data to save energy if the channel is busy.
3.  **Flush/Discard:** Safely drop low-priority data that has expired.

### The Reward Model (The "Teacher")
The AI is taught using a complex scoring system:
*   **Positive Reward:** For successfully clearing high-priority queues.
*   **Negative Penalty:** Severe points deducted for letting "Spike" data pile up or for letting a node run out of battery.
*   **Efficiency Bonus:** Points for maintaining a clean buffer with minimal energy waste.

---

## 🌍 Simulation Setup
*   **Network Size:** Supports scaling from 50 to 100 dense nodes.
*   **Protocols:** Uses **LrWpan** (IEEE 802.15.4) for low-power wireless communication.
*   **Analysis:** Generates full **PCAP** logs and convergence plots (`wsn_dqn_convergence.png`) to visualize exactly how the AI improves over 200 training episodes.

---

## ❤️ Simple Summary
Imagine a team of 100 soldiers (sensors) in a forest. Each soldier has a walkie-talkie with a dying battery. Instead of everyone shouting every time they see a bird, our AI teaches them to:
1.  **Stay silent** if nothing interesting is happening (Normal).
2.  **Report periodically** if they see something noteworthy (Critical).
3.  **Shout immediately** if they see a sudden fire (Spike).
4.  **Turn off the radio** more often as their battery gets lower to survive longer.

**This is what our project achieves: An autonomous, energy-efficient, and safety-aware intelligent network.**
