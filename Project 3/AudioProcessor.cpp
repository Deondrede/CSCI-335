#include "AudioProcessor.h"

std::vector<short> AudioProcessor::Compress(const audio_t& audio, short threshold, float rate){
    if (audio.empty() == true){
        return audio;
    }
    if (threshold < 0){
        throw std::invalid_argument("\"threshold\" must be of type unsigned integer.");
    }
    if (rate < 1){
        throw std::invalid_argument("\"rate\" must be no less than 1.0");
    }

    audio_t newAudio;
    for (int i = 0; i < audio.size(); i++){
        if (audio[i] >= threshold){
            newAudio.push_back(std::round((float)((audio[i] - threshold) / rate) + threshold));
        }
        else if (audio[i] <= threshold * -1){
            int newSample = (std::round((float)(((audio[i] * -1) - threshold) / rate) + threshold));
            newSample *= -1;
            newAudio.push_back(newSample);
        }
        else {
           newAudio.push_back(audio[i]); 
        }
    }
    return newAudio;
}

std::vector<short> AudioProcessor::CutOutSilence(const audio_t& audio, short level, int silenceLength){
    if (audio.empty() == true){
        return audio;
    }
    if (level < 0){
        throw std::invalid_argument("\"level\" must be of type unsigned integer.");
    }
    if (silenceLength < 1){
        throw std::invalid_argument("The \"silenceLength\" must be no less than 1");
    }
    
    audio_t newAudio;
    std::queue<short> audioQueue;
    for (int i{0}; i < audio.size(); i++){
        if (audio[i] <= level && audio[i] >= (level * -1)){
            audioQueue.push(audio[i]);      //Push all values outside the level range into the queue
        }
        else {
            if (audioQueue.size() < silenceLength){     //Check if the the queue has values that weren't in the silenceLength then add
                while (!audioQueue.empty()){
                    newAudio.push_back(audioQueue.front());
                    audioQueue.pop();
                }
            }
            newAudio.push_back(audio[i]);           //Push values outside of the range into the new vector and empty the queue
            while (!audioQueue.empty()){
                audioQueue.pop();
            }
        }
    }
    if (audioQueue.empty() == false){           //If the queue still has values after the loop ends check to see if they need to be added to the vector
        int counter = audioQueue.size();
        while (!audioQueue.empty()){
            if(counter < silenceLength){
                newAudio.push_back(audioQueue.front());
                audioQueue.pop();
            }
            else{
                audioQueue.pop();
            }
        }
    }
    return newAudio;
}

std::vector<short> AudioProcessor::StretchTwice(const audio_t& audio){
    if (audio.empty() == true){
        return audio;
    }
    else if (audio.size() == 1){
        return audio;
    }
    audio_t newAudio;
    short average = 0;
    for (int i{0}; i < audio.size(); i++){
        if (i+1 >= audio.size()){           //Check necessary to make sure that the loop doesn't check outside the scope of the vector
            newAudio.push_back(audio[i]);   //If it is, then add the last value and exit the loop
            break;                      
        }
        average = std::round(((float)audio[i] + (float)audio[i+1]) / 2);
        newAudio.push_back(audio[i]);
        newAudio.push_back(average);
    }
    return newAudio;
}

std::vector<short> AudioProcessor::Normalize(const audio_t& audio, short normalizeTarget){
    if (audio.empty() == true){
        return audio;
    }
    if (normalizeTarget < 1){
        throw std::invalid_argument("\"normalizeTarget\" must be no less than 1");
    }

    std::priority_queue<short> maxZero;
    std::priority_queue<short, std::vector<short>, std::greater<short>> minZero;
    for (int i{0}; i < audio.size(); i++){
        maxZero.push(audio[i]);
    }
    
    for (int i{0}; i < audio.size(); i++){
        minZero.push(audio[i]);
    }

    if (maxZero.top() == 0 && minZero.top() == 0){  //Check to ensure that the vector isn't filled with zeroes
        return audio;
    }

    audio_t newAudio;
    std::priority_queue<short> normalizeQueue;      //Created a max heap to find the largest value in the vector
    for (int i{0}; i < audio.size(); i++){
        normalizeQueue.push(std::abs(audio[i]));
    }
    float factor = (float)normalizeTarget / (float)normalizeQueue.top();        //Some algebra to find the factor that I need to multoply the entire vector by
    for (int i{0}; i < audio.size(); i++){
        newAudio.push_back(std::round((float)audio[i] * factor));
    }
    return newAudio;
}
