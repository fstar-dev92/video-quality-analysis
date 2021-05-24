<h1 align="center">Bageera</h1>
<p align="center">
  <strong>Black and Freeze Detection</strong>
</p>

<h3 align="center">
  <a href="https://www.amagi.com/">CREATE</a>
  <span> · </span>
  <a href="https://www.amagi.com/">DISTRIBUTE</a>
  <span> · </span>
  <a href="https://www.amagi.com/">MONETIZE</a>
</h3>

---

## Structure

| Codebase              |      Description          |
| :-------------------- | :-----------------------: |
| [inc](inc)        	|     Header Files          |
| [src](src)  		    |     Source code           |
| [ut](ut)      	    |  	  Unit Tests            |



## Contributions

Bageera is open to contributions, but I recommend creating an issue or replying in a comment to let me know what you are working on first that way we don't overwrite each other.



## Bageera Reviewers

Contributors helping to review/merge pull requests:

- [@Hamza](https://github.com/hamza-u)
- [@Swapnil](https://github.com/swapnild18)
- [@Bvs](https://github.com/bvs-amagi)

## How to run locally

1. Clone this repository 
2. Go to Bageera directory
3. Create docker image using following command
	`docker build . -t <image_name>`
4. Run the following command to create reports freeze or black detection for video files in your test folder
	
	`docker run -v <test_directory>:/data/input -v <reports_directory>:/data/output -it --rm <image_name> ./ut/<freeze_detect_fsm.sh/black_detect.sh> /data/input /data/output`
	* test_directory - directory with videos to be checked
	* reports_directory - directory in which the reports are to be generated
	* image_name - name of image created in previous step
	* freeze/black file 
		* for freeze detection : freeze_detect_fsm.sh
		* for black detection : black_detect.sh
		
