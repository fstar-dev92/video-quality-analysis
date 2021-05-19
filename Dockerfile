FROM centos:7.3.1611

COPY ["./" , "/home/work/"]
WORKDIR "/home/work"
RUN wget https://registrationcenter-download.intel.com/akdlm/irc_nas/17769/l_BaseKit_p_2021.2.0.2883_offline.sh
RUN yum groups mark install "Development Tools" 
RUN yum groups mark convert "Development Tools"
RUN yum groupinstall "Development Tools" -y
RUN sh /home/work/l_BaseKit_p_2021.2.0.2883_offline.sh -s -a --silent --eula accept
RUN yum install cmake -y
RUN yum install epel-release -y
RUN yum localinstall --nogpgcheck https://download1.rpmfusion.org/free/el/rpmfusion-free-release-7.noarch.rpm https://download1.rpmfusion.org/nonfree/el/rpmfusion-nonfree-release-7.noarch.rpm -y
RUN yum install ffmpeg -y
RUN cd /home/work/amg_video_freeze/ut && cmake . && make
