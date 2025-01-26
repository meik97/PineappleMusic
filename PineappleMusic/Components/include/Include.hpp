#pragma once

//Base
#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>

static unsigned long long BASE = reinterpret_cast<unsigned long long>(GetModuleHandle(NULL));
static unsigned long long AMPServicesBase = reinterpret_cast<unsigned long long>(GetModuleHandle(L"AMP.Services.dll"));
static unsigned long long MicrosoftUIXamlBase = reinterpret_cast<unsigned long long>(GetModuleHandle(L"Microsoft.UI.Xaml.dll"));

