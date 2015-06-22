classdef BotMaster < handle
    %BOTMASTER Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        
        bots
        h
    end
    
    methods
        
        function obj = BotMaster(h)
            obj.h = h;
        end
        
        function C = getC(obj, i, t)
            C = [];
            for j = obj.bots
                if i ~= j && norm(j.pos(t)-i.pos(t)) <= obj.h
                    C = union(C, j);
                end
            end
        end
    end
    
end

