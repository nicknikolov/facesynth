# Facesynth

Facesynth is an openFrameworks project.  
It uses the following addons:  
- ofxCV  
- ofxFaceTracker  
- ofxGui  
- ofxMaxim  
- ofxOpenCv  

Facesynth is based on Kyle McDonalds' Face tracking work and Mick Grierson's Maximliam DSP library.
All I have done is implement a simple formant filter that generates different vowels.
Facesynth interpolates between those vowels based on your mouth shape so when someone sees yo
moving your mouth in a particular way and hears the sound produced, it looks somewhat realistic

You can set different vowels using keyboard shortcuts.  
'c' => from vowel A, 'C' => to vowel A  
'v' => from vowel E, 'V' => to vowel E  
'b' => from vowel I, 'B' => to vowel I  
'n' => from vowel O, 'N' => to vowel O  
'm' => from vowel U, 'M' => to vowel U  
Future version could include more sophisticated and realistic looking lerping...

You can also change the scale you are currently "singing" in.
By default its C major. For those familiar, just like in a DAW program like Ableton,
the keyboard is mapped like a real piano where  
a = C  
w = C#  
s = D  
e = D#  
d = E  
f = F  
t = F#  
g = G  
y = G#  
h = A  
u = A#  
j = B  

By pressing the lower case of any of those letter, you switch to the minor version of that
particular scale. Pressing the upper case letter, you will get the major scale.
'G' for example gives you G Major. 'u' gives you A sharp minor.
Again, just like Ableton, you can use 'z' to tranpose down and 'x' to tranpose up.
To spice it up, moving your eyebrows up will give the sound a slight vibrato.
Lastly, you can play around with the delay to create a nicer and more interesting melody sounds.
The Facesynth is most fun when you "sing" over some harmony and match the key or chords
by selecting the right scale, or "singing" over your favoure tunes.

There is a short demo here:  
https://vimeo.com/149336885 (sound is direct but horrible latency and FPS)  
https://vimeo.com/149336884 (sound comes from mic)  
password for both is: av

Unfortunately, I had technical problems with making nice videos :(  
They really need to be better but I had trouble routing openFrameworks to soundflower,
also recoding the screen dropped my fps quite low and I had big latency...  
Will make nicer videos when I figure this out.


