function init(start, goal, traj)
% init(start, goal, traj)
%   init creates a new plot and draws the start locations, goals and
%   trajectories

newplot;
circle(start, 0.5, [1, 0, 0]);
circle(goal, 0.5, [1, 1 , 1]);
[N, ~] = size(start);

for n = 1:N
    line(traj.desPos{n}(:, 1), traj.desPos{n}(:, 2));
end

axis equal

end

