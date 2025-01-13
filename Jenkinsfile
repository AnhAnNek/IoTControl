pipeline {
    agent any

    environment {
        // Docker image names
        DOCKER_IMAGE_BACKEND = 'iot-web-server'
        DOCKER_IMAGE_FRONTEND = 'iot-web-ui'
        SERVER_IP = '192.168.31.90'
        DEPLOY_DIR = '~/deployment-dir' // Define the path to your deploy folder
        SSH_USER = 'your-username'
    }

    stages {
        stage('Clone Repository') {
            steps {
                // Clone both frontend and backend projects from GitHub
                git url: 'https://github.com/AnhAnNek/IoTControl', branch: 'main'
            }
        }

        stage('Build Backend (Spring Boot)') {
            steps {
                dir('iot-web-server') {
                    // Build Spring Boot JAR
                    sh 'docker build -t $DOCKER_IMAGE_BACKEND .'  // Build Docker image for backend
                }
            }
        }

        stage('Build Frontend (ReactJS)') {
            steps {
                dir('iot-web-ui') {
                    sh 'docker build -t $DOCKER_IMAGE_FRONTEND .'  // Build Docker image for frontend
                }
            }
        }

        stage('Deploy to Server') {
            steps {
                sshagent(['your-ssh-credentials-id']) {
                    // Deploy the Docker images to the remote server
                    sh """
                    docker save $DOCKER_IMAGE_BACKEND | ssh user@$SERVER_IP 'docker load'
                    docker save $DOCKER_IMAGE_FRONTEND | ssh user@$SERVER_IP 'docker load'
                    ssh user@$SERVER_IP 'docker-compose -f $DEPLOY_DIR/docker-compose.yml up -d'
                    """
                }
            }
        }
    }

    post {
        success {
            echo 'Deployment completed successfully!'
        }
        failure {
            echo 'Deployment failed. Please check the logs.'
        }
    }
}

