

                        UE4 First Person CPP Neuron Template
                        ====================================
					
                                      v0.1.0
                           
                          Created by Heiko Fink aka Cyx
                                           

                           
Contents:
---------
        I.    Description
        II.   Installation
        III.  Configuration
        IV.   Contact, Donation
        V.    Greetings        
        VI.   LICENSE
        VII.  Third party copyrights and trademarks


I. Description
==============
This template is based on the Unreal (R) Engine 4 First Person C++ 4.9 template and 
enhances it with the support for the Perception Neuron (TM) motion capture suit.


II. Installation
================
* Make sure that you have Unreal Engine 4.9.1 or newer installed.
* Make also sure that you have Visual Studio 2013 installed.
  The community edition is free for individual developers.
  Don't be afraid if you aren't familiar with C++ and don't want to change the code.
  Unreal automatically compiles the code while opening the project for the first time.
  You can just close Visual Studio and save the changes after starting.
* Download the template under: "https://github.com/Cyx69/TP_ThirdPersonNeuron/releases".
* Unpack the file to your UE4 template folder, for e.g. "C:\Program Files\Unreal Engine\4.9\Templates\TP_ThirdPersonNeuron".
  Make sure that the project files are placed and visible under "...\Templates\TP_ThirdPersonNeuron" after unzip.
* Launch Unreal Editor and click on "New project".
* Under C++ you should now see a template called "TP Neuron".
* Create a new project with the above template selected.


III. Configuration
==================
The template was developed and tested with Unreal Engine 4.9.1 and Axis Neuron 3.5.19.1225.
The C++ development was done with MS Visual Studio Community 2013.

To connect from the template to the Axis Neuron SW following settings should be done in the 
Axis Neuron SW:
- Open File|Settings dialog
- Activate under Broadcasting BVH and choose port 7001
- Configure under Output Format:
  * Frequency reducing: 1
  * Block type: String
  * Rotation: YXZ
  * Activate 'Displacement'
  * Deactivate 'Reference'

Per default the template connects to 'localhost' (127.0.0.1) and to port 7001.
You can change these settings in the Controller Blueprint if you want to use a separate PC or other ports.
And you can add further controllers if you want to retrieve motion data from further suits or BVH server.


IV. Contact, Donation
=====================
Heiko Fink aka Cyx
EMail: hfinkdeletemenospam@web.de

If you find the template or parts of it useful, please support me and
the further development and make a donation here:
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DYZEA9A3MLA5Y


V. Greetings
============
Many thanks to:
* Everybody which helped me to improve and develop this template further
* Epic for providing the world best game engine for free
* Noitom for providing us Kickstarter Backers a motion capture suit for such an affordable price
* Blender Foundation for developing one of the best open source tools out there
* Makehuman for their awesome free tool to create human characters
* Microsoft for providing a free C++ IDE for individual developers


VI. LICENSE
===========
The MIT License (MIT)

Copyright (c) 2015 Heiko Fink

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE



VII. Third party copyrights and trademarks
==========================================
Perception Neuron (TM) is a trademark of Beijing Noitom Technology Ltd.
Unreal (R) Engine is under the copyright of Epic Games, Inc.
