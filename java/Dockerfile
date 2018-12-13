FROM glassfish:latest

#ENV http_proxy http://172.17.0.1:5865
#ENV https_proxy http://172.17.0.1:5865
#COPY apt.conf /etc/apt/apt.conf

RUN apt-get -y update
RUN apt-get -y install git curl imagemagick

RUN echo "Deploy application"
COPY dist/ImageUpload_-_Java.war /root
RUN asadmin start-domain && asadmin deploy /root/ImageUpload_-_Java.war && asadmin create-http-listener --listeneraddress 0.0.0.0 --listenerport 8000 --defaultvs server externalListener

EXPOSE 8000
#EXPOSE 4848

