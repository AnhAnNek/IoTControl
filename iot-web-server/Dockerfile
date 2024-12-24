# Stage 1: Build the application
FROM maven:3.8.3-openjdk-17 AS build
WORKDIR /iot-web-server

# Copy only the project definition files
COPY pom.xml .

# Download dependencies and cache them
RUN mvn dependency:go-offline -B

COPY src ./src
RUN mvn clean package -DskipTests

# Stage 2: Set up the runtime environment
FROM openjdk:17-jdk-slim AS runtime
WORKDIR /iot-web-server

# Copy the built JAR file from the previous stage
COPY --from=build /iot-web-server/target/iot-web-server.jar .

# Expose the port on which the application will run
EXPOSE 8001

ENTRYPOINT ["java", "-jar", "iot-web-server.jar"]