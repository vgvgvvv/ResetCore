//
// Created by 董宸 on 2018/4/18.
//

#include <thread>
#include <iostream>
#include "FMod/fmod.hpp"
#include "FMod/fmod_studio.hpp"

int main(){

    FMOD::Studio::System *system = nullptr;
    auto result = FMOD::Studio::System::create(&system);
    if(FMOD_RESULT::FMOD_OK != result){
        std::cout << "Error Line:" << __LINE__ << " Type:" << result << std::endl;
        return 1;
    }


    system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);

    FMOD::Studio::Bank* masterBank;
    result = system->loadBankFile("Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
    if(FMOD_RESULT::FMOD_OK != result){
        std::cout << "Error Line:" << __LINE__ << " Type:" << result << std::endl;
        return 1;
    }
    FMOD::Studio::Bank* stringBank;
    result = system->loadBankFile("Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringBank);
    if(FMOD_RESULT::FMOD_OK != result){
        std::cout << "Error Line:" << __LINE__ << " Type:" << result << std::endl;
        return 1;
    }

    FMOD::Studio::Bank* BGMBank;
    result = system->loadBankFile("BGM.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &BGMBank);
    if(FMOD_RESULT::FMOD_OK != result){
        std::cout << "Error Line:" << __LINE__ << " Type:" << result << std::endl;
        return 1;
    }

    FMOD::Studio::EventDescription* description;
    result = system->getEvent("event:/BGM/BGM_Title_DengLu", &description);
    if(FMOD_RESULT::FMOD_OK != result){
        std::cout << "Error Line:" << __LINE__ << " Type:" << result << std::endl;
        return 1;
    }

    FMOD::Studio::EventInstance *eventInstance;
    result = description->createInstance(&eventInstance);
    if(FMOD_RESULT::FMOD_OK != result){
        std::cout << "Error Line:" << __LINE__ << " Type:" << result << std::endl;
        return 1;
    }

    result = eventInstance->start();
    if(FMOD_RESULT::FMOD_OK != result){
        std::cout << "Error Line:" << __LINE__ << " Type:" << result << std::endl ;
        return 1;
    }

    while (true){
        system->update();

    }
    system->release();
    return 0;
}

