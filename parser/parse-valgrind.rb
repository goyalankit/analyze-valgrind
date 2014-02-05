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

CSV.foreach("valgrind-output") do |row|
    next if row[0].include?("==")
    next if row[0].include?("user") || row[0].include?("input")
    if row[0].include?("varinfo")
        s = row[0].split(":")
        variables << VarInfo.new(s[1], s[3], s[2])
    end    
end

CSV.foreach("valgrind-output") do |row|
    next if row[0].include?("==")
    next if row[0].include?("user") || row[0].include?("input")
    next if row[0].include?("varinfo")
    next if row.nil? || row[0].nil? || row[0] == ""

    variables.each do |v|
        address = (row[0].split(" ")[1]).to_d
        if((address >= v.base_address.to_d) && (address <= (v.base_address.to_d + 4 * (v.size.to_i - 1))))
            p row
        end
    end
end


