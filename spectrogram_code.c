% 여러 WAV 파일을 선택하기
[files, path] = uigetfile('*.wav', 'Select WAV files', 'MultiSelect', 'on');

% 사용자가 파일 선택을 취소한 경우 예외 처리
if isequal(files, 0)
    disp('No file selected.');
    return;
end

% 파일이 하나만 선택된 경우, cell array로 변환하지 않아도 되므로 체크
if ischar(files)
    files = {files};
end

% 스펙트로그램 생성 시 사용할 파라미터 설정
windowSize = 256;
noverlap = 250;
nfft = 256;

% 가장 긴 시간 길이를 찾기 위한 변수 초기화
maxLength = 0;

% 모든 파일의 길이를 확인하여 가장 긴 길이를 계산
for i = 1:length(files)
    filePath = fullfile(path, files{i});
    [y, Fs] = audioread(filePath);
    
    % 하나의 채널만 선택 (왼쪽 채널)
    y = y(:, 1);
    
    % 시간 길이를 계산
    duration = length(y) / Fs;
    if duration > maxLength
        maxLength = duration;
    end

% 1. 설정 및 초기화
normalBatteryFolder = 'C:\work\new';
wasteBatteryFolder = 'C:\work\old';

% 2. 이미지 데이터스토어 생성
imds = imageDatastore({normalBatteryFolder, wasteBatteryFolder}, ...
    'IncludeSubfolders', true, 'LabelSource', 'foldernames');

% 3. 데이터 분할 (훈련 데이터와 테스트 데이터)
[imdsTrain, imdsTest] = splitEachLabel(imds, 0.7, 'randomized');

% 4. 이미지 전처리 (크기 조정) - augmentedImageDatastore 사용
inputSize = [224 224 3];
augimdsTrain = augmentedImageDatastore(inputSize, imdsTrain);
augimdsTest = augmentedImageDatastore(inputSize, imdsTest);

% 5. CNN 모델 아키텍처 정의
layers = [
    imageInputLayer(inputSize)
    
    convolution2dLayer(3, 64, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer
    
    maxPooling2dLayer(2, 'Stride', 1)
    
    convolution2dLayer(3, 64, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer
    
    maxPooling2dLayer(2, 'Stride', 1)
    
    convolution2dLayer(3, 64, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer
    
    fullyConnectedLayer(2) % 클래스 수 (0, 1)
    softmaxLayer
    classificationLayer];

% 6. 학습 옵션 설정
options = trainingOptions('sgdm', ...
    'InitialLearnRate', 0.001, ...
    'MaxEpochs', 40, ...
    'MiniBatchSize', 32, ...
    'Shuffle', 'every-epoch', ...
    'ValidationData', augimdsTest, ...
    'ValidationFrequency', 30, ...
    'Verbose', false, ...
    'Plots', 'training-progress');

% 7. CNN 모델 학습
net = trainNetwork(augimdsTrain, layers, options);

% 8. 모델 평가
YPred = classify(net, augimdsTest);
YTest = imdsTest.Labels;

% 정확도 계산
accuracy = sum(YPred == YTest) / numel(YTest);
fprintf('Test accuracy: %.2f%%\n', accuracy * 100);

% 9. 혼동 행렬 출력
confusionchart(YTest, YPred);

% 1. 설정 및 초기화
normalBatteryFolder = 'C:\work\new';
wasteBatteryFolder = 'C:\work\old';

% 2. 이미지 데이터스토어 생성
imds = imageDatastore({normalBatteryFolder, wasteBatteryFolder}, ...
    'IncludeSubfolders', true, 'LabelSource', 'foldernames');

% 3. 데이터 분할 (훈련 데이터와 테스트 데이터)
[imdsTrain, imdsTest] = splitEachLabel(imds, 0.7, 'randomized');

% 4. 이미지 전처리 (크기 조정) - augmentedImageDatastore 사용
inputSize = [224 224 3];
augimdsTrain = augmentedImageDatastore(inputSize, imdsTrain);
augimdsTest = augmentedImageDatastore(inputSize, imdsTest);

% 5. CNN 모델 아키텍처 정의
layers = [
    imageInputLayer(inputSize)
    
    convolution2dLayer(3, 64, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer
    
    maxPooling2dLayer(2, 'Stride', 1)
    
    convolution2dLayer(3, 64, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer
    
    maxPooling2dLayer(2, 'Stride', 1)
    
    convolution2dLayer(3, 64, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer
    
    fullyConnectedLayer(2) % 클래스 수 (normal, waste)
    softmaxLayer
    classificationLayer];

% 6. 학습 옵션 설정
options = trainingOptions('sgdm', ...
    'InitialLearnRate', 0.001, ...
    'MaxEpochs', 40, ...
    'MiniBatchSize', 32, ...
    'Shuffle', 'every-epoch', ...
    'ValidationData', augimdsTest, ...
    'ValidationFrequency', 30, ...
    'Verbose', false, ...
    'Plots', 'training-progress');

% 7. CNN 모델 학습
net = trainNetwork(augimdsTrain, layers, options);

% **여기서 학습된 모델을 저장**
save('yourModel.mat', 'net');  % 모델을 저장하는 부분

% 사전에 학습된 머신러닝 모델 불러오기
% 'yourModel.mat'을 실제로 학습한 모델 파일로 대체하세요.
load('yourModel.mat', 'trainedModel'); % 모델이 'trainedModel'로 저장되었다고 가정

% 마이크 입력 설정
fs = 44100; % 샘플링 레이트 (마이크 사양에 맞게 조정)
recObj = audiorecorder(fs, 16, 1); % 16비트, 모노

disp('배터리를 던져 소리를 기다리세요...'); -> input
recordblocking(recObj, 1); % 1초간 오디오 녹음
audioData = getaudiodata(recObj);

% 오디오 데이터를 스펙트로그램으로 변환하여 특징 추출
windowLength = 256;
overlapLength = 128;
nfft = 256;
[S, F, T, P] = spectrogram(audioData, windowLength, overlapLength, nfft, fs);

% 파워 스펙트로그램을 로그 스케일로 변환 (모델이 로그 스펙트로그램을 사용했다면)
logP = 10*log10(P);

% 스펙트로그램을 특징 벡터로 변환 (모델 입력에 맞게 조정)
featureVector = logP(:)';

% 학습된 모델로 분류
[label, score] = predict(trainedModel, featureVector);

% 분류 결과 출력
if strcmp(label, 'normalBattery')
    disp('이 배터리는: 일반 배터리로 분류되었습니다.');
else
    disp('이 배터리는: 폐건전지로 분류되었습니다.');
end

% 선택적으로 예측 점수 출력
disp(['예측 신뢰도: ', num2str(max(score))]);
