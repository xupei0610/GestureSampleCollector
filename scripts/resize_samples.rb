#!/usr/bin/ruby

# 
# This script resize the sampled PGM files.
# It will make a directory named under the folder storing the PGM directory and put all new images into it.
#
# This script depends on libvips and ruby-vips.
#


require 'vips'

SAMPLE_DIR   = '../samples'
SAMPLE_NAMES = [*'0'..'9', *'A'..'Z', *'Z0'..'Z3', *'bak0'..'bak9']
TARGET_SIZE  = [128, 64, 32]

Vips::Image.instance_eval do
  def zoom im_file, new_size, method = :linear
    im = Vips::Image.new_from_file im_file
    s = im.height > im.width ? new_size.fdiv(im.height) : new_size.fdiv(im.width)
    new_im = im.resize s, :kernel => method
    x, y = if new_im.width > new_im.height
             [0, ((new_size - new_im.height)/2).floor]
           else
             [((new_size - new_im.width)/2).floor, 0]
           end
    new_im.embed x, y, new_size, new_size, :background=>[0,0,0,1]
  end
end

Vips::Image.class_eval do
  def pgm_save name, opts = {}
    filename = Vips::filename_get_filename name
    option_string = Vips::filename_get_options name
    Vips::call_base "VipsForeignSavePpm", self, option_string, [filename, opts]
    write_gc
  end
end

if __FILE__ == $0

  progress_bar = -> current, total do
                   print "\r      %s/%d: |%s| %0.2f%%" % [
                           current.to_s.rjust(total.to_s.size),
                           total,
                           '#'*((50*current/total).floor).ljust(50),
                           100 * current.to_f / total
                         ]
                   print "\n" if current == total
                 end

  SAMPLE_NAMES.each do |g|
    d = File.absolute_path File.join(SAMPLE_DIR, g, 'PGM')
    puts "Enter dir: #{d}"
    next unless Dir.exist? d

    files = Dir.entries(d).select do |f|
              not f.start_with? '.'
            end
    total = files.count

    TARGET_SIZE.each do |s|
      tar_dir  = File.absolute_path File.join(SAMPLE_DIR, g, s.to_s)
      tmp_file = File.join tar_dir, '.tmp'
      Dir.mkdir tar_dir unless Dir.exist? tar_dir
      puts "Target dir: #{tar_dir}"

      i = 0
      files.each do |f|
        i += 1
        Vips::Image.zoom(File.join(d, f), s).pgm_save tmp_file
        File.rename tmp_file, File.join(tar_dir, f)
        progress_bar.call i, total
      end
    end
  end

end