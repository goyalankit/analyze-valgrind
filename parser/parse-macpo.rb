#require 'pry'
require 'csv'

CSV.foreach("macout") do |row|
  csv = "#{row}"
  a = csv.split(":")[1]
  p a.split("+") unless a.nil?
#   puts csv
end
