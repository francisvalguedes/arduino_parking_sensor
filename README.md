# Arduino Parking Sensor

## Reverse engineering the communication protocol of a low-cost 4-sensor ultrasonic automotive parking sensor for arduino connection.

In development by: Francisval Guedes Soares

## the low cost parking sensor system:

<figure>
<center><img src="./figures/sensor_sistem.jpeg" width="800"></center>
<figcaption align = "center"><b>Fig.1 - Sensor sistem</b></figcaption>
</figure>

<figure>
<center><img src="./figures/pcb.jpeg" width="800"></center>
<figcaption align = "center"><b>Fig.2 - PCB</b></figcaption>
</figure>

## Protocol

Start bit length ~ 978us, low length ~ 112 us, clock length ~ 77us, bit length ~ 133us hight=1 and low=0 

<figure>
<center><img src="./figures/Protocol.jpg" width="800"></center>
<figcaption align = "center"><b>Fig.3 - Protocol</b></figcaption>
</figure>


Bit   | value
--------- | ------
0 - 4 | unknown
5 | presence of an obstacle
6 - 7 | sensor number
8 - 16 | sensor data
