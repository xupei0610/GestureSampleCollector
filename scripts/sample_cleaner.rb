#!/usr/bin/ruby -w

NAME = 'K'

BMP_DIR = "../samples/#{NAME}/BMP"
PGM_DIR = "../samples/#{NAME}/PGM"

Dir.class_eval do
  def sync! dir
    (self.entries-Dir.entries(dir)).each do |f|
      file = File.join self.path, f
      File.delete file
      puts "Deleted #{File.absolute_path file}"
    end
  end
end

if __FILE__ == $0
  puts ''
  puts '=========== Sample Cleaner ============'
  puts '==                                   ==' 
  puts '== This script will clean samples by =='
  puts '== synchronizing BMP and PGM folder. =='
  puts '==                                   =='
  puts '=========== Author:  Pei Xu ==========='
  puts ''

  # SAMPLE_DIR   = '../samples'
  # SAMPLE_NAMES = [*'0'..'9', *'A'..'Z', *'Z0'..'Z3', *'bak0'..'bak9']
  # SAMPLE_NAMES.each do |g|
  #   db = File.join SAMPLE_DIR, g, 'BMP'
  #   dp = File.join SAMPLE_DIR, g, 'PGM'
  #   Dir.new(db).sync! dp
  #   Dir.new(dp).sync! db
  # end
  
  
  raise "Invalid BMP dir: #{BMP_DIR}" unless Dir.exist? BMP_DIR
  raise "Invalid PGM dir: #{PGM_DIR}" unless Dir.exist? PGM_DIR

  entries = [BMP_DIR, PGM_DIR].map do |d|
    Dir.entries d
  end
  common_entries = entries[0] & entries[1]
  puts "BMP dir: #{File.absolute_path BMP_DIR}"
  puts "         #{entries[0].count-common_entries.count} will be deleted."
  puts "PGM dir: #{File.absolute_path PGM_DIR}"
  puts "         #{entries[1].count-common_entries.count} will be deleted."
  exit if entries[0].count-common_entries.count == 0 and entries[1].count-common_entries.count == 0
  
  while true
    print 'Synchronize? (Y/N) '
    res = gets.chomp
    exit if res == 'n' or res == 'N'
    break if res == 'y' or res == 'Y'
  end

  Dir.new(BMP_DIR).sync! PGM_DIR
  Dir.new(PGM_DIR).sync! BMP_DIR

end


