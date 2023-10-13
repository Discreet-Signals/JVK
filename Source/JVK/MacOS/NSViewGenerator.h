/*
  ==============================================================================

    NSViewGenerator.h
    Created: 9 Oct 2023 4:46:14pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
namespace jvk
{

class NSViewGenerator
{
public:
    // Constructor and destructor if necessary
    NSViewGenerator();
    ~NSViewGenerator();
    
    bool isValid();
    void* create();
    void release();
private:
    void* ptr;
};

} // jvk
