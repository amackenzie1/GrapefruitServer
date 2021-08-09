I tried to assemble a very basic server that would connect to a browser and serve html, javascript and images. Now http and browsers make a lot more sense in my mind. Compile server with gcc, and then run ./server 8888 (or whatever port you prefer!)

The name field triggers a get request; all the other buttons are handled in javascript. I couldn't figure out for a while why modulo wasn't working in my javascript code: took me some time to realize I was using sprintf on the html file and C was getting rid of all the modulo signs. Probably... not how real server operate.
