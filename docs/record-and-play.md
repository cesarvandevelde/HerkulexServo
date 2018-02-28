# Record and Play

<div style="width: 60%; margin-bottom: 2em;">
  <div style='position: relative; padding-bottom: 56.25%;'>
    <iframe src='https://gfycat.com/ifr/DismalLoathsomeAmericancurl'
            frameborder='0'
            scrolling='no'
            width='100%'
            height='100%'
            style='position: absolute; top: 0; left: 0'
            allowfullscreen>
    </iframe>
  </div>
</div>

This example sketch lets you record and play back motion patterns. Press the
record button and move the servo around. The sketch will read and store 5
seconds of position data. Press the play button to make the servo repeat the
motion pattern.

**Note:** This example depends on the [Bounce2 library][bounce2], which needs to be
installed separately.

**Note:** This sketch uses two external buttons. Hook the record button up
between pin 4 and ground, and the play button between pin 5 and ground. No
external resistors are needed.

[bounce2]: https://github.com/thomasfredericks/Bounce2

[include, lang:"C++"](../examples/RecordAndPlay/RecordAndPlay.ino)
