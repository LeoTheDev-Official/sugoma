#pragma once
#include "engine/module.h"

using namespace sugoma::core;

class AppModule : public Module 
{
public:
	AppModule();
	~AppModule();
	void OnAttach() override;
	void OnDetach() override;
	bool Validate() override;
	void Tick() override;

};