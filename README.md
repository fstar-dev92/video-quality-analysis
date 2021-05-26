<h1 align="center">BAGEERA</h1>
<h2 align="center">Black and Freeze detection</h2>

<h4 align="center">
  <a href="https://www.amagi.com/">CREATE</a>
  <span> · </span>
  <a href="https://www.amagi.com/">DISTRIBUTE</a>
  <span> · </span>
  <a href="https://www.amagi.com/">MONETIZE</a>
</h4>

---

## Structure

| Codebase              |      Description          |
| :-------------------- | :-----------------------: |
| [inc](inc)        	|     Header Files          |
| [src](src)  		    |     Source code           |
| [ut](ut)      	    |  	  Unit Tests            |




## Bageera Reviewers

Contributors helping to review/merge pull requests:

- [@BVS](https://github.com/bvs-amagi)
- [@Swapnil](https://github.com/swapnild18)
- [@Hamza](https://github.com/hamza-u)
- [@Islam]()


## How Bageera works

Bageera is a project which provides following functionalities :
* Freeze detection in a video 
    * Current frame is compared with the previous frame to check whether they are similar.
    * If similarity persists for a certain period of time (freeze_threshold) then the video clip is considered as freeze.
    * Report generated contains the details of video_file_name, Start pts (ns), End pts (ns), Duration (ms).
* Black detection in a video
    * Luma of the image is checked to find whether a frame is black or non black.
    * If black persists for a certain period of time (black_threshold) then the video clip is considered as black.
    * Report generated contains the details of video_file_name, Start pts (ns), End pts (ns), Duration (ms).

## Optimization of Bageera

* Skip_line parameter is used to skip few lines for each horizontal line processed in each frame to reduce computation.
* ROI (Region Of Interest) is set to full screen.
* Threshold of 120 ms is used to get out of freeze or black state when non-freeze or non-black frames are observed respectively. 
* Updation of reference frame is changed to detect slow changes (eg: slow motion videos) in freeze.
* FSM (Finite State Machine) is used to manage states in black and freeze.
* Slice based detection is used in black to detect small non-black part (eg: credits, logo, fade in, fade out  etc..) in a frame.

## How to run locally

1. Clone this repository 
2. Go to Bageera directory
3. Create docker image using following command
	`docker build . -t <image_name>`
4. Run the following command to create reports freeze or black detection for video files in your test folder
	
	`docker run -v <test_directory>:/data/input -v <reports_directory>:/data/output -it --rm <image_name> ./ut/<freeze_detect_fsm.sh/black_detect.sh> /data/input /data/output <black/freeze threshold>`
	* test_directory - directory with videos to be checked
	* reports_directory - directory in which the reports are to be generated
	* image_name - name of image created in previous step
	* freeze/black file 
		* for freeze detection : freeze_detect_fsm.sh
		* for black detection : black_detect.sh
    * freeze threshold - no of milliseconds freeze should be shown
    * black threshold  - no of milliseconds black should be shown
	
## Getting started

Below are few commands to generate reports on videos in ./ut/test folder with bageera as current working directory

* docker build . -t sample_image 
* #To generate freeze report on the video files in test folder
* docker run -v $PWD/ut/test:/data/input -v $PWD/ut/:/data/output -it --rm sample_image $PWD/ut/freeze_detect_fsm.sh /data/input /data/output 1000
* #To generate black report on the video files in test folder
* docker run -v $PWD/ut/test:/data/input -v $PWD/ut/:/data/output -it --rm sample_image $PWD/ut/black_detect.sh /data/input /data/output 1000

