function run_demoVelodyne (base_dir,calib_dir)
% KITTI RAW DATA DEVELOPMENT KIT
% 
% Demonstrates projection of the velodyne points into the image plane
%
% Input arguments:
% base_dir .... absolute path to sequence base directory (ends with _sync)
% calib_dir ... absolute path to directory that contains calibration files

% clear and close everything
close all; dbstop error; clc;
disp('======= KITTI DevKit Demo =======');

% options (modify this to select your sequence)
if nargin<1
  base_dir  = '/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync';
end
if nargin<2
  calib_dir = '/home/husong/kitti/2011_09_26';
end
cam       = 0; % 0-based index
frame     = 0; % 0-based index

% load calibration
calib = loadCalibrationCamToCam(fullfile(calib_dir,'calib_cam_to_cam.txt'));
Tr_velo_to_cam = loadCalibrationRigid(fullfile(calib_dir,'calib_velo_to_cam.txt'));

% compute projection matrix velodyne->image plane
R_cam_to_rect = eye(4);
R_cam_to_rect(1:3,1:3) = calib.R_rect{1};
P_velo_to_img = calib.P_rect{cam+1}*R_cam_to_rect*Tr_velo_to_cam

% load and display image
img = imread(sprintf('%s/image_%02d/data/%010d.png',base_dir,cam,frame));
fig = figure('Position',[20 100 size(img,2) size(img,1)]); axes('Position',[0 0 1 1]);
imshow(img); hold on;

% load velodyne points
fid = fopen(sprintf('%s/velodyne_points/data/%010d.bin',base_dir,frame),'rb');
velo = fread(fid,[4 inf],'single')';
velo = velo(1:5:end,:); % remove every 5th point for display speed

% load 3d bounding boxes from FRIBS
fid=fopen('/home/husong/kitti/outfile','r');
% i=1;
% format long;
pos=[0 0 0];
dim=[0 0 0];
sizeA=[ 6 Inf ];
pos=fscanf(fid,'%f %f %f\n%f %f %f',sizeA);
pos=pos';
idx = pos(:,1)<5;
pos(idx,:)=[];
dim=pos(:,4:6);
pos=pos(:,1:3)
for i=1:size(dim,1)
    bbox(i*8-8+1,:)=[pos(i,1)-dim(i,1)/2 pos(i,2)-dim(i,2)/2 pos(i,3)+dim(i,3)/2];
    bbox(i*8-8+2,:)=[pos(i,1)-dim(i,1)/2 pos(i,2)-dim(i,2)/2 pos(i,3)-dim(i,3)/2];
    bbox(i*8-8+3,:)=[pos(i,1)-dim(i,1)/2 pos(i,2)+dim(i,2)/2 pos(i,3)+dim(i,3)/2];
    bbox(i*8-8+4,:)=[pos(i,1)-dim(i,1)/2 pos(i,2)+dim(i,2)/2 pos(i,3)-dim(i,3)/2];
    bbox(i*8-8+5,:)=[pos(i,1)+dim(i,1)/2 pos(i,2)-dim(i,2)/2 pos(i,3)+dim(i,3)/2];
    bbox(i*8-8+6,:)=[pos(i,1)+dim(i,1)/2 pos(i,2)-dim(i,2)/2 pos(i,3)-dim(i,3)/2];
    bbox(i*8-8+7,:)=[pos(i,1)+dim(i,1)/2 pos(i,2)+dim(i,2)/2 pos(i,3)+dim(i,3)/2];
    bbox(i*8-8+8,:)=[pos(i,1)+dim(i,1)/2 pos(i,2)+dim(i,2)/2 pos(i,3)-dim(i,3)/2];
end
fclose(fid);

% remove all points behind image plane (approximation
idx = velo(:,1)<5;
velo(idx,:) = [];

% project to image plane (exclude luminance)
velo_img = project(velo(:,1:3),P_velo_to_img);
for i=1:size(bbox,1)
    for j=1:8
        bbox_img=project(bbox(i,j,:),P_velo_to_img);
    end
    bboxctr_img=project(bbox(i,9,:),P_velo_to_img);
end
bbox_img=project(bbox(:,:),P_velo_to_img);
bboxctr_img=project(pos(:,:),P_velo_to_img);
% plot points
cols = jet;
for i=1:size(bbox_img,1)
%   col_idx = round(64*5/velo(i,1));
  plot(bbox_img(i,1),bbox_img(i,2),'o','LineWidth',4,'MarkerSize',1,'Color','g');
end

for i=1:size(bboxctr_img,1)
%   col_idx = round(64*5/velo(i,1));
  plot(bboxctr_img(i,1),bboxctr_img(i,2),'o','LineWidth',4,'MarkerSize',2,'Color','r');
end