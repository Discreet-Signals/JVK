# JVK
 Vulkan integration for JUCE plugins.
 <img width="734" alt="Screenshot 2023-10-15 at 7 47 59 PM" src="https://github.com/ZGgof1999/JVK/assets/9949963/ebb0b266-1a13-44ed-915c-b1e1c8e46d7b">
 Background and text rendered with JUCE.
 Triangles rendered with transparency, from two seperate components added to single vulkan renderer component
 
 ## Installation
 If not using the projucer, just use JVK as any other juce module!
 
 ### Projucer
 1. Install Vulkan (Not Bundled)
 2. Ensure VULKAN\_SDK environment is set
 3. run 'sh include_vulkan.sh [path/to/project] [optional VULKAN_SDK_PATH]'
 
 This will process your .jucer to add the JVK module, add necessary search paths, and link necesarry external libraries. If you add a new exporter, just run the script again!
 
 [Warning: If you are linking multiple external libraries, the automatic .jucer processor will not work, and it is recomended you use CMAKE to setup your project and link vulkan.]
