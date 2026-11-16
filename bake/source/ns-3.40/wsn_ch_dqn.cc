#include "ns3/core-module.h"
#include "ns3/opengym-module.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("WSN_CH_DQN");
NS_LOG_UNCOND("🚀 NS3 CH-DQN Starting...");  // FIRST LINE

#define NUM_CLUSTERS 8
#define NUM_SINKS 3
#define STATE_SIZE 12
#define ACTION_SIZE 7

class ClusterHead {
public:
    double queue_length = 8.0;
    double wait_time = 2.0;
    double residual_energy = 85.0;
    double sink_delay[NUM_SINKS] = {1.2, 1.5, 1.8};
    double link_quality[NUM_SINKS] = {0.9, 0.85, 0.92};
    int priority_level = 1;
    double congestion = 0.3;
    int high_priority_packets = 2;
    int low_priority_packets = 6;
};

ClusterHead ch[NUM_CLUSTERS];
Ptr<OpenGymInterface> gym;
uint32_t episode_count = 0;
uint32_t port = 5555;

NS_LOG_UNCOND("📊 Initializing " << NUM_CLUSTERS << " clusters...");

void ResetEnvironment() {
    srand(time(NULL) + episode_count);
    for(int i = 0; i < NUM_CLUSTERS; i++) {
        ch[i].queue_length = 5.0 + rand() % 8;
        ch[i].wait_time = 1.0 + rand() % 3;
        ch[i].residual_energy = 70.0 + rand() % 25;
        ch[i].priority_level = rand() % 3;
        ch[i].congestion = 0.2 + rand() % 40 / 100.0;
        ch[i].high_priority_packets = 1 + rand() % 3;
        ch[i].low_priority_packets = 3 + rand() % 5;
        for(int j = 0; j < NUM_SINKS; j++) {
            ch[i].sink_delay[j] = 1.0 + 0.5 * j + rand() % 20 / 100.0;
            ch[i].link_quality[j] = 0.7 + rand() % 30 / 100.0;
        }
    }
    episode_count++;
    NS_LOG_UNCOND("🚀 CH-DQN EPISODE " << episode_count << " | Clusters: " << NUM_CLUSTERS);
}

Ptr<OpenGymSpace> GetObservationSpace() {
    std::vector<uint32_t> shape = {STATE_SIZE};
    NS_LOG_UNCOND("📐 Creating observation space...");
    return CreateObject<OpenGymBoxSpace>(0.0f, 1000.0f, shape, "float32");
}

Ptr<OpenGymSpace> GetActionSpace() {
    NS_LOG_UNCOND("🎮 Creating action space...");
    return CreateObject<OpenGymDiscreteSpace>(ACTION_SIZE);
}

Ptr<OpenGymDataContainer> GetObservation() {
    int active_ch = rand() % NUM_CLUSTERS;
    NS_LOG_UNCOND("👁️  Getting observation for CH[" << active_ch << "]");
    
    Ptr<OpenGymBoxContainer<double>> box = CreateObject<OpenGymBoxContainer<double>>();
    box->AddValue(ch[active_ch].queue_length);
    box->AddValue(ch[active_ch].wait_time);
    box->AddValue(ch[active_ch].residual_energy);
    box->AddValue(ch[active_ch].sink_delay[0]);
    box->AddValue(ch[active_ch].sink_delay[1]);
    box->AddValue(ch[active_ch].sink_delay[2]);
    box->AddValue(ch[active_ch].link_quality[0]);
    box->AddValue(ch[active_ch].link_quality[1]);
    box->AddValue(ch[active_ch].link_quality[2]);
    box->AddValue(ch[active_ch].congestion);
    box->AddValue((double)ch[active_ch].priority_level);
    box->AddValue((double)ch[active_ch].high_priority_packets);
    
    return box;
}

float GetReward() {
    int active_ch = rand() % NUM_CLUSTERS;
    float reward = 0.0f;
    reward += 20.0f * std::max(0.0f, (float)(10.0 - ch[active_ch].queue_length));
    reward -= (float)(100.0 - ch[active_ch].residual_energy) * 0.12f;
    reward -= (float)ch[active_ch].wait_time * 8.0f;
    reward -= (float)ch[active_ch].congestion * 15.0f;
    if(ch[active_ch].high_priority_packets > 0) reward += 25.0f;
    return reward;
}

bool ExecuteActions(Ptr<OpenGymDataContainer> action) {
    Ptr<OpenGymDiscreteContainer> discrete = DynamicCast<OpenGymDiscreteContainer>(action);
    uint32_t a = discrete->GetValue();
    int active_ch = rand() % NUM_CLUSTERS;
    
    NS_LOG_UNCOND("🤖 CH[" << active_ch << "] A:" << a 
                << " Q:" << ch[active_ch].queue_length 
                << " E:" << ch[active_ch].residual_energy 
                << " HP:" << ch[active_ch].high_priority_packets);
    
    // Action logic (simplified)
    switch(a) {
        case 0: case 1: case 2:
            ch[active_ch].queue_length = std::max(1.0, ch[active_ch].queue_length - 4);
            ch[active_ch].residual_energy -= 2.5 + a * 0.3;
            break;
        default:
            ch[active_ch].residual_energy -= 0.5;
            break;
    }
    return true;
}

int main(int argc, char *argv[]) {
    NS_LOG_UNCOND("🎯 Initializing OpenGym on port " << port);
    
    gym = CreateObject<OpenGymInterface>(port);
    NS_LOG_UNCOND("✅ OpenGym interface created!");
    
    gym->SetGetObservationSpaceCb(MakeCallback(&GetObservationSpace));
    gym->SetGetActionSpaceCb(MakeCallback(&GetActionSpace));
    gym->SetGetObservationCb(MakeCallback(&GetObservation));
    gym->SetGetRewardCb(MakeCallback(&GetReward));
    gym->SetExecuteActionsCb(MakeCallback(&ExecuteActions));
    
    NS_LOG_UNCOND("🔌 Callbacks registered!");
    
    ResetEnvironment();
    NS_LOG_UNCOND("🔄 Starting simulation loop...");
    
    Simulator::Schedule(Seconds(0.5), [](){
        NS_LOG_UNCOND("[OpenGym] Listening on port 5555...");
    });
    
    Simulator::Run();
    gym->NotifySimulationEnd();
    Simulator::Destroy();
    
    NS_LOG_UNCOND("🏁 Simulation ended!");
    return 0;
}

