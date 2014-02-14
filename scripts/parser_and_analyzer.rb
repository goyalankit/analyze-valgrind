#!/work/02681/ankitg/programs/bin/ruby

#
# Define the base parser.
#

class BaseParser
    attr_accessor :filename, :results

    def initialize filename
        @filename = filename
        @results = {}
    end

    #override this method in case of different format of file
    def get_var_name line
        line.strip.split(":")[1].split("+")[0].strip
    end

    def get_comparison_string line
        raise "Hey you need to redefine this method"
    end

    def parse_file
        File.readlines(@filename).each do |line|
            next if line.strip == "" || line.nil?
            var_name = get_var_name line
            @results[var_name] = [] if @results[var_name].nil?
            @results[var_name] << get_comparison_string(line)
        end
        @results
    end
end

#
# Parse the valgrind file
#


class Valgrind < BaseParser
    def get_comparison_string line
        line.strip.split(":").map{|i| i.strip}[0...-1].join
    end
end

#
# Parse the Macpo file
#

class Macpo < BaseParser
    def get_comparison_string line
        line.strip.split(":").map{|i| i.strip}[0..-1].join
    end
end

class Result
    attr_accessor :error_count, :indexes_not_matched

    def initialize
        @error_count = 0
        @indexes_not_matched = []
    end

    def update_result match, index
        unless match
            @error_count += 1
            @indexes_not_matched << index
        end
    end

    def display
        if @error_count == 0
            puts "Both the traces are same!"
        else
            puts "Traces are not same: "
            puts "Error Count: #{@error_count}"
        end
    end
end

class Analyzer

    attr_accessor :vresults, :mresults

    ret = lambda { |message| puts message; return }

    def initialize vresults, mresults
        @vresults = vresults
        @mresults = mresults
    end

    def analyze
        ret("Number of variables are not same in both the files") if @vresults.keys.size != @mresults.size        
        result = Result.new
        @vresults.keys.each do |key|            
            @vresults[key].each_with_index do |vvalue, index|
                puts "Warning: Number of values don't match for variable - #{key}" if @vresults[key].size != @mresults[key].size
                match = false                
                @mresults[key].drop(index).each do |mvalue|
                    if(mvalue == vvalue)
                        match = true
                        break
                    end
                end
                result.update_result match, index
                match = false if match
            end
        end
        result.display
    end
end
