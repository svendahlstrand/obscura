# obscura

Salvage your beautiful photographs from a Game Boy Camera save file.

![](doc/img/obscura.jpg)

Photos snapped with a Game Boy Camera is saved on a battery backed RAM on the cartridge. There's [lots of ways for you to backup that RAM][1], and thus your photos, to a save file on your computer.

Once you have that save file, pass it to `obscura` and you'll soon have all your snaps in [PGM image format][2] on your computer.

## Extract photos from save file

Just pass your save file as a parameter to `obscura` like this:

```sh
$ obscura game-boy-camera.sav
```

You'll end up with all 30 photos in PGM format:

```sh
$ ls
game-boy-camera.sav  image-1.pgm
image-2.pgm          image-3.pgm
[...]
```

Of course it's easy to convert to any format you want. Here's an example using [ImageMagick][3]:

```sh
mogrify -format png *.pgm
```

[1]: https://www.reddit.com/r/Gameboy/comments/4w8xjf/guide_how_to_backup_your_game_save_files/
[2]: http://netpbm.sourceforge.net/doc/pgm.html
[3]: https://www.imagemagick.org/
