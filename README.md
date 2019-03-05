# PluginTimeMeasurer
Measuring plugin performance

**What is this?**  
This is a tool that measures the time it takes for a plugin to process an audio buffer block.  
The time it takes to process one block is divided by the time **between** two blocks which is similar to how much CPU the block took to process.

**How to use?**  
The tool is a plugin itself and should be compiled using a JUCE project (see dependencies).  
The plugin should be inserted before and after the plugin (or plugin chain) that needs to be measured.  
The setting ("off", "start", "end") determines whether the plugin is before or after the chain.  
The ID (1 - 16) makes it possible to measure multiple plugins/chains by giving each plugin/chain an unique, up to 16 different, ID's.  
The plugin with the "end" setting will visualize the graph.  

**Dependencies:**  
JUCE: https://github.com/WeAreROLI/JUCE

**Notes:**  
This tool measures time, and cannot always be trusted.
For instance, the block buffer size can be variable because a DAW might decide to process the blocks in chunks, for example to smooth automation. This leads to incorrect results.
Also there are some differences between Windows and Mac in the stability, speed and wake up time of the CPU.  
Anyway, there are more examples of where this developer tool should be used carefully. Don't trust it blindly.  

**Credits:**  
[**Martijn Zwartjes**](https://www.112db.com/) - Initiative, idea and concept    
[**Jelle Bakker**](https://www.jb-audio.com/) - Coding    
