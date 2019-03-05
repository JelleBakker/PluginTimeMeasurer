# PluginTimeMeasurer
Measuring plugin performance

What is this?  
This is a tool that measures the time it takes for a plugin to process an audio buffer block.  
The time it takes to process one block is divided by the time **between** two blocks which is similar to how much CPU the block took to process.

How to use?  
The tool is a plugin itself and should be compiled using a JUCE project (see dependencies).  
The plugin should be inserted before and after the plugin (or plugin chain) that needs to be measured.  
The setting ("off", "start", "end") determines whether the plugin is before or after the chain.  
The ID (1 - 16) makes it possible to measure multiple plugins/chains by giving each plugin/chain an unique, up to 16 different, ID's.  

Dependencies:
- JUCE: https://github.com/WeAreROLI/JUCE

Initiative, idea and concept: **Martijn Zwartjes**- https://www.112db.com/  
Coding: **Jelle Bakker** - https://www.jb-audio.com/  
