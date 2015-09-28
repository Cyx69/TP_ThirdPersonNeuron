

                        UE4 First Person CPP Neuron Template
                        ====================================
					
                                      v0.2.0
                           
                          Created by Heiko Fink aka Cyx
                                           

                           
Contents:
---------
        I.    Description
        II.   Requirements
        III.  Installation
        IV.   Configuration
        V.    Contact, Donation
        VI.   Greetings        
        VII.  LICENSE
        VIII. Third party copyrights and trademarks


I. Description
==============
This template is based on the Unreal (R) Engine 4 First Person C++ 4.9 template and 
enhances it with the support for the Perception Neuron (TM) motion capture suit.

Perception Neuron (TM) is a motion capture suit from Noitom financed by a Kickstarter campaign last year:
https://www.kickstarter.com/projects/1663270989/project-perception-neuron
Further details about the suit here: https://neuronmocap.com

Please use this thread to discuss the technical details about this template:
https://forums.unrealengine.com/showthread.php?85318-Perception-Neuron-Template&p=384125#post384125

How it is done:
- UE4 connects over a TCP connection to the BVH server and retrieves the raw ASCII BVH data
- Each tick one motion line is parsed and locally stored
- The stored rotation and transition data is then recalculated to map between the different coordinate systems and rotation orders.
- The animation graph reads this information and replaces the rotation and transition for each bone.

To get a better performance the network handling and calculation is done in C++.
A custom blueprint node retrieves this data from the C++ world and pumps it into your animation blueprint.
To handle and configure each network connection separately a custom controller actor was implemented.


II. Requirements
================
* Make sure that you have Unreal Engine 4.9.1 or newer installed.
* Make also sure that you have Visual Studio 2013 installed.
  The community edition is free for individual developers.
  Don't be afraid if you aren't familiar with C++ and don't want to change the code.
  Unreal automatically compiles the code while opening the project for the first time.
  You can just close Visual Studio and save the changes after starting.
* Axis Neuron SW Player (Free version is available here: https://neuronmocap.com/downloads)
  or alternatively you can use my minimal BVH send server here:
  https://github.com/Cyx69/bvhsend/releases


III. Installation
=================
* Download the template under: "https://github.com/Cyx69/TP_ThirdPersonNeuron/releases".
* Unpack the file to your UE4 template folder, for e.g. "C:\Program Files\Unreal Engine\4.9\Templates\TP_ThirdPersonNeuron".
  Make sure that the project files are placed and visible under "...\Templates\TP_ThirdPersonNeuron" after unzip.
* Launch Unreal Editor and click on "New project".
* Under C++ you should now see a template called "TP Neuron".
* Create a new project with the above template selected.


IV. Configuration
=================
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


V. Contact, Donation
====================
Heiko Fink aka Cyx
EMail: hfinkdeletemenospam@web.de

If you find the template or parts of it useful, please support me and
the further development and make a donation here:
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DYZEA9A3MLA5Y


VI. Greetings
=============
Many thanks to:
* Everybody which helped me to improve and develop this template further
* Epic for providing the world best game engine for free
* Noitom for providing us Kickstarter Backers a motion capture suit for such an affordable price
* Blender Foundation for developing one of the best open source tools out there
* Makehuman for their awesome free tool to create human characters
* Microsoft for providing a free C++ IDE for individual developers


VII. LICENSE
============
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



VIII. Third party copyrights and trademarks
===========================================
Perception Neuron (TM) is a trademark of Beijing Noitom Technology Ltd.
Unreal (R) Engine is under the copyright of Epic Games, Inc.
