import React, { useState } from 'react';
import './Login.css';

const ipAddress = "192.168.65.91"; // Adresse IP à utiliser

const Login = ({ onLoginSuccess }) => {
    const [email, setEmail] = useState('jeanleduc@gmail.com');
    const [password, setPassword] = useState('varna');
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState('');

    const handleLogin = async (e) => {
        e.preventDefault();
        setLoading(true); // Démarre l'animation de chargement

        const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/login.php?mail=${email}&pass=${password}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ email, password }),
        });

        setLoading(false); // Arrête l'animation de chargement

        const data = await response.json();
        if (data.success) {
            console.log(data);
            onLoginSuccess(data.userId);
        } else {
            setError('Login failed');
        }
    };

    return (
        <div className="container">
            <div className="form-container">
                <form onSubmit={handleLogin}>
                    <div>
                        <label>Email</label>
                        <input 
                            type="email" 
                            value={email} 
                            onChange={(e) => setEmail(e.target.value)} 
                            required 
                        />
                    </div>
                    <div>
                        <label>Password</label>
                        <input 
                            type="password" 
                            value={password} 
                            onChange={(e) => setPassword(e.target.value)} 
                            required 
                        />
                    </div>
                    <button type="submit">
                        {loading ? <div className="loader"></div> : 'Login'}
                    </button>
                </form>
                {error && <p className="error-message">{error}</p>}
            </div>
        </div>
    );
};

export default Login;
