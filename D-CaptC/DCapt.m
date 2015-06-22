function traj = DCapt(start,goal,R, h)
%DCAPT Summary of this function goes here
%   Detailed explanation goes here

N = size(start, 1);

if ~isequal(size(start),size(goal))
    error('start and goal locations must be the same size');
end

startMat = pdist2(start,start);
if any(startMat(triu(true(size(startMat)),1))<2*sqrt(2)*R)
    error('start vertex seperation must be at least 2*sqrt(2)R');
end
goalMat = pdist2(goal,goal);
if any(goalMat(triu(true(size(goalMat)),1))<2*sqrt(2)*R)
    error('goal vertex seperation must be at least 2*sqrt(2)R');
end

master = BotMaster(h);
bots = cell(N, 1);


for i = 1:N
    newBot = Bot(1.0, start(i, :), goal(i, :), master);
    bots{i} = newBot;
   
end

for i = 0:0.01:1.0
    for j = 1:N
        bots{j}.step(i);
    end
end

traj = cell(N, 1);
for i = 1:N
    traj{i} = bots{i}.traveled;
end



    

end

