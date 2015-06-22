classdef Bot < handle
    %BOT Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        traj;
        timeSweep;
        
        update;
        cLast;
        goal;
        master;
        id;
        tf;
        tl;
        traveled;
        
    end
    
    methods
        
        function obj = Bot(tf, start, goal, botMaster)
            
            obj.timeSweep = 0.0:0.01:tf;
            obj.cLast = [];
            obj.tl = 0;
            obj.tf = tf;
            obj.goal = goal;
            obj.update = [];
            botMaster.bots = union(botMaster.bots, obj);
            obj.master = botMaster;
            obj.traveled = zeros(length(obj.timeSweep), length(start));
            obj.traveled(1, :) = start;
            computeTraj(obj, goal, start, 0);
            
            
        end
        
       
        function x = pos(obj, t)
         
            i = find(abs(obj.timeSweep-t) < .005);
            x = obj.traj(i, :);
         
        end
        
        function display(obj)
            fprintf('%d\n', obj.id);
            
        end
            
        function step(obj, t)
            
            
            iLast = find(abs(obj.timeSweep-obj.tl) < 0.005);
            iCur = find(abs(obj.timeSweep-t) < 0.005);
            
            obj.tl = t;
            
            obj.traveled(iLast:iCur, :) = obj.traj(iLast:iCur, :);
            
            C = obj.master.getC(obj, t);
            temp = setdiff(C, obj.cLast);
            
            obj.update = union(temp, obj.update);
            
            obj.update = intersect(C, obj.update);
            
            while ~isempty(obj.update)
                j = obj.update(1);
                
            
                u = Bot.u(obj, j, t);
                w = Bot.w(obj, j);
                
                if u*w' < 0
                    newGoal = j.goal;
                    j.swap(obj.goal, t);
                    obj.goal = newGoal;
                    obj.update = C;
                    obj.computeTraj(newGoal, obj.pos(t), t);
                end
                
                
                obj.update = setdiff(obj.update, [j]);
               
            end
            
            obj.cLast = C;
            
            N = length(obj.timeSweep);
            
            obj.traveled(max([fix(t/obj.tf*N), 1]), :) = obj.pos(t);
        end
        
        function swap(obj, newGoal, t)
            obj.goal = newGoal;
            obj.computeTraj(newGoal, obj.pos(t), t);
            obj.update = obj.master.getC(obj, t);
        end
        
        function computeTraj(obj, g, xc, t)
            frac = (obj.timeSweep - t)./(obj.tf-t);
            temp = bsxfun(@times, frac', g-xc);
            obj.traj = bsxfun(@plus, temp,xc);
        end
    end
    
    methods(Static)
        
        function d = dist(i, j, t)
            d = norm(pos(i, t)-pos(j,t));
        end
        
        function v = r(i, j, t)
            v = j.goal - pos(i, t);
        end
        
        function v = u(i, j, t)
            v = pos(j, t) - pos(i, t);
        end
        
        function v = w(i, j)
            v = j.goal - i.goal;
        end
    end
    
end

