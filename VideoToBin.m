clc;
clear;

% ���峣��
VIDEO_NAME = 'BadApple.mp4';

IMAGE_WIDTH = 128;
IMAGE_HEIGHT = 64;
PATH = './images/';
POSTFIX = '.bmp';

numOfImages = 9480; % video.NumberOfFrames

% % ������Ƶ
% video = VideoReader(VIDEO_NAME);
% numOfImages = video.NumberOfFrames; % 9480
% for i = 1 : numOfImages
%     frame = read(video, i);
%     frame = imresize(frame, [IMAGE_HEIGHT IMAGE_WIDTH]); % ������СΪ128x64
%     name = sprintf('%04d', i - 1);
%     path = [PATH, name, POSTFIX];
%     imwrite(frame, path); % ����֡ΪBMPͼƬ
% end


% ��ȡ������ͼƬ
imgs = zeros(numOfImages, IMAGE_WIDTH * IMAGE_HEIGHT);
for i = 1 : numOfImages
    A = logical([]);
    name = sprintf('%04d', i - 1);
    path = [PATH, name, POSTFIX];
    img = imread(path);
    bw = imbinarize(img); % ��im2bw����һ��㲻ͬ
    for j = 1 : 8
        for k = 1 : 128
            B = bw(1 + 8 * (j - 1) : 8 + 8 * (j - 1), k)';
            A = [A, B];
        end
    end
    imgs(i, :) = A;
end

% ����
bin = uint8(zeros(numOfImages, IMAGE_WIDTH * IMAGE_HEIGHT / 8));
for i = 1 : numOfImages
    for j = 1 : IMAGE_WIDTH * IMAGE_HEIGHT / 8
        for k = 0 : 7
            bin(i, j) = bin(i, j) + imgs(i, k + 1 + (j - 1) * 8) * 2^k;
        end
    end
end

% ����Ϊbin�ļ�
bin = bin';
fp = fopen('BadApple.bin', 'wb');
fwrite(fp, bin, 'uint8');
fclose(fp);
