This distribution contains makefiles for VMS. It is a very preliminary release
and many tests have still to be done.

The compilation was tested with
    -OpenVMS Alpha 7.2
    -DECC 6.0
    -Compac C++ 6.2
    
To get every thing compiled you'll need to have installed
    -Bison
    -Flex
    -Sed
  These programs are all on the OpenVMS freeware CD and can be downloaded from
  http://www.openvms.digital.com/freeware/
  At the moment you'll have to hack the descrip.mms files to include the right
  locations of the packages, but i intend to make this better in future.
  
Currently only the MOTIF GUI vcan be build by typing
  MMS/MACRO=(__MOTIF__=1)
 in the main directory of the distribution.
 
It automatically makes the library in [.lib] and links the following samples
 [.samples.menu]menu.exe
 


Finally :
 I like the idea of Robert Roebling that CD's with classical music should be
send to the authors.


>-----------------------------------------------------------------------------<

  Jouk Jansen
		 
  joukj@hrem.stm.tudelft.nl


  Technische Universiteit Delft        tttttttttt  uu     uu  ddddddd
  Nationaal centrum voor HREM          tttttttttt  uu     uu  dd    dd
  Rotterdamseweg 137                       tt      uu     uu  dd     dd
  2628 AL Delft                            tt      uu     uu  dd     dd
  Nederland                                tt      uu     uu  dd    dd
  tel. 31-15-2781536                       tt       uuuuuuu   ddddddd

>-----------------------------------------------------------------------------<
