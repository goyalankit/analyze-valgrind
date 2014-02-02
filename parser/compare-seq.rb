#!/bin/ruby

require 'csv'

ms = [] #macpo sequence
vs = [] #valgrind sequence

CSV.foreach("mac-seq") do |row|
    ms << row
end

CSV.foreach("val-seq") do |row|
    vs << row
end

current_index = -1

found = false

ms_count = {}
vs_count = {}

ms.each do |m|
    ms_count[m] = (ms_count[m] || 0) + 1
end

vs.each do |v|
    vs_count[v] = (vs_count[v] || 0) + 1
end

#p ms_count
#p vs_count
count = 0

vs_count.each do |k,v|
    if(ms_count[k] != vs_count[k])
        count += 1
    end
end

p "number of different series - #{count}"
p "macpo sequence count - #{ms_count.length}"
p "valgrind sequence count - #{vs_count.length}"

=begin
ms.each_with_index do |m,oi|    
#   p "value #{m} index #{oi}"
    found = false
    vs.each_with_index do |v,i|
        next if(i < current_index)
        next if v!=m
        if v==m
            current_index = i
            found = true
            break;
        end
    end 
    p "no match found for outer index #{oi} and value #{m}" unless found
end
=end
