function [traj, unassigned_goals] = CaptBasic(start,goal,R,t0,tf,dt)
% traj = CaptBasic(start,goal,R)
%
%   Capt plans for N robots to navigate to N goals
%
%   Input start and goal locations must be N x d matrices for N robots that
%       live in d dimensions
%
%   The separation between all start locations must be at least 2*sqrt(2)*R
%
% traj = CaptBasic(start,goal,R,t0,tf,dt)
%   t0, tf, and dt are optional inputs to specify the time range

addpath('../hungarian')

% input error checking
if (size(start) > size(goal))
    error('start and goal locations must be the same size');
end

if nargin == 3
    timeSweep = 0:.01:1;
    frac = timeSweep;
elseif nargin ==6
    timeSweep = t0:dt:tf;
    frac = (timeSweep-t0)./(tf-t0);
else
    error('Either 3 or 6 input arguments required');
end

% ensure all start and goal locations satisfy separation requirements
startMat = pdist2(start,start);
if any(startMat(triu(true(size(startMat)),1))<2*sqrt(2)*R)
    error('start vertex seperation must be at least 2*sqrt(2)R');
end
goalMat = pdist2(goal,goal);
if any(goalMat(triu(true(size(goalMat)),1))<2*sqrt(2)*R)
    error('goal vertex seperation must be at least 2*sqrt(2)R');
end


%compute distance matrix between all start and goal loations
distMat = pdist2(start,goal);

% compute optimal assignment using hungarian algorithm
[assignment, cost]= assignmentoptimal(distMat.^2);

if(size(start,1) > size(goal,1))
  % create permutation matrix for this assignment and align goals
  phi = CreatePhi(assignment);
  alignedGoals = phi*goal;
else
  alignedGoals = goal(assignment,:);
end

% desired vector of desired motion
vectors = alignedGoals-start;


% compute trajectories using bsxfun for efficiency
for num = 1:size(start,1)
    traj.desPos{num} = bsxfun(@times,vectors(num,:),frac.')+bsxfun(@plus,start(num,:),zeros(size(frac)).');
end
traj.time = timeSweep.';

goal(assignment,:) = [];
unassigned_goals = goal;
