function DrawCurrent(hObject, start, goal, traj)
% DrawCurrent(hObject, start, goal, traj)
%   DrawCurrent refreshes the plot and draws the position of the robots at
%   a time determined by the state of the slider
init(start, goal, traj);

n = get(hObject,'Value');
[s, ~] = size(traj.time);
t = max([fix(n*s), 1]);

[~, N] = size(traj.desPos);

Pos = traj.desPos{1}(t, :);

for i = 2:N
    Pos = [Pos;traj.desPos{i}(t, :)];
end

circle(Pos, 0.5, [0, 0, 0]);

