import React from 'react';
import ReactDOM from 'react-dom/client';
import './index.css';
import App from './App';
import {WebSocketProvider} from "./Providers/WebSocketProvider";

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(
  <React.StrictMode>
    <WebSocketProvider>
      <App />
    </WebSocketProvider>
  </React.StrictMode>
);