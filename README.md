## One GEGL node  (gegl:gegl) makes an entire node based plugin

This is not a plugin for users to use, its just a boring color overlay and outline. But what makes it interesting is its made in a entirely different way then how I make my GEGL plugins.

X's AI Grok made a GEGL plugin that does an color overlay and a outline with sliders by calling gegl:gegl like `gegl:gegl string=" color-overlay value=%s dropshadow x=%f y=%f radius=%f grow-radius=%f opacity=%f color=white "` and then updating its paramters with sliders

Pic attached of the success

![image](https://github.com/user-attachments/assets/0851d2a6-27bb-4ee3-b88e-53d78ff408d7)


I was always aware of gegl:gegl and use its strings a lot in my plugins but I did not know it was possible to integrate sliders and other things to properties inside the gegl:gegl string.

It clearly is not intended by GEGL developers and is a "crude hack" but hey it works and in the future it could become easier.
