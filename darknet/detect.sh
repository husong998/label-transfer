for file in `ls /home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/image_00/data/`; do
	./darknet detect cfg/yolov3.cfg yolov3.weights /home/husong/darknet/2011_09_26_drive_0001_sync/image_00/data/$file >> results/result
	mv predictions.png results/$file
	echo >> results/result
done
