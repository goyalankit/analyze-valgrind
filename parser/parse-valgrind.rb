require 'csv'
require 'pry'

# create a method in string class to covert hex to integer
class String
    def to_d
        self.to_i(16) #convert hex(16) to decimal(10)
    end
end

class VarInfo
    attr_accessor :name, :base_address, :size

    def initialize name, base_address, size
        @name = name
        @base_address = base_address
        @size = size
    end
end

variables = []

File.readlines('valgrind-output').each do |line|
    line = line.strip
    next if line.nil? || line == ""
    next if line.include?("==")
    next if line.include?("user") || line.include?("input")
    if line.include?("varinfo")
        s = line.split(":")
        variables << VarInfo.new(s[1], s[3], s[2])
    end    
end

File.readlines('valgrind-output').each do |line|
    line = line.strip
    next if line.include?("==")
    next if line.include?("user") || line.include?("input")
    next if line.include?("varinfo")
    next if line.nil? || line.nil? || line  == ""
    
    variables.each do |v|
        s = line.split(" ")[1]
        address, size = s.split(",")
        if((address.to_d >= v.base_address.to_d) && (address.to_d <= (v.base_address.to_d + 4 * (v.size.to_i - 1))))
           index_accessed = (address.to_d - v.base_address.to_d) / 4
           num_indexes = size.to_d / 4
           num_indexes.times do |n|
               p "#{v.name},#{index_accessed + n}"
           end
        end
    end
end
