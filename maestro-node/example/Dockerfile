FROM node:6

WORKDIR /app

ENV MAESTRO_URL maestro-api.maestro

COPY ./example/package.json /app/package.json
RUN npm install
COPY ./example/app.js /app/app.js

CMD env MAESTRO_URL=$MAESTRO_URL node /app/app.js
