#!/usr/bin/ruby

#
# This script using tk to randomly display samples.
#

require 'tk'
require 'tkextlib/tkimg/bmp'

SAMPLE_DIR   = '../../Data/PX50Gestures/BMP'
SAMPLE_NAMES = [*'0'..'9', *'A'..'Z', *'Z0'..'Z3', *'bak0'..'bak9']
PER_LINE     = 10

if __FILE__ == $0
  TkOption.add '*tearOff', 0
  # Tk::Tile::Style.theme_use 'clam'

  root = TkRoot.new do
    title 'Gesture Thumbnails (Pei Xu)'
    resizable false, false
  end
  
  load_gestures = -> do
      SAMPLE_NAMES.count.times do |i|
      d = File.join SAMPLE_DIR, SAMPLE_NAMES[i]
      row, col = i/PER_LINE*2, i%PER_LINE
      TkLabel.new root do
        text SAMPLE_NAMES[i]
        font TkFont.new :weight=>'bold'
        grid :row=>row, :column=>col, :pady=>[20, 0]
      end
      next unless Dir.exist? d
      samples = Dir.entries(d).select do |f|
                  not f.start_with? '.'
                end
      next if samples.count < 1
      img = TkPhotoImage.new
      img.copy TkPhotoImage.
                 new(:file=>File.join(d, samples[Random.rand samples.count])),
               :subsample=>[5, 5],
               :shrink=>true
      TkLabel.new root do
        image img
        grid :row=>row+1, :column=>col
      end
    end
  end

  root.menu TkMenu.new(:border=>0).add(
              :cascade,
              :label => 'File',
              :menu  => TkMenu.new(:relief=>'sunken') do
                          add :command, :label=>'Reload', :command=>load_gestures
                        end
            )
  load_gestures.call
                
  Tk.mainloop
end
