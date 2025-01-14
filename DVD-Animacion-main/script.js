// Inicializar posición del SVG
const svgElement = document.getElementById('bouncing-svg');
const container = document.getElementById('container');
let dx = 3; // Velocidad en X
let dy = 3; // Velocidad en Y
let position = { x: 0, y: 0 }; // Posición inicial del SVG
let moving = true; // Estado de movimiento inicial como `true`
let initialized = false; // Estado de inicialización de posición aleatoria

// Verificar si el SVG está en el DOM
if (svgElement) {
    // Establecer un tamaño predeterminado para los SVG
    svgElement.style.width = '100px'; // Ancho del SVG
    svgElement.style.height = '100px'; // Alto del SVG

    const updatePosition = () => {
        if (!moving) return; // Si no está moviéndose, no actualizamos la posición

        const rect = container.getBoundingClientRect();
        const svgWidth = svgElement.offsetWidth;
        const svgHeight = svgElement.offsetHeight;

        position.x += dx;
        position.y += dy;

        // Comprobaciones de colisión con los bordes
        if (position.x + svgWidth >= rect.width || position.x <= 0) {
            dx *= -1; // Invertir dirección en X
            playSound(svgElement.getAttribute('data-audio')); // Reproducir sonido al tocar el límite en X
        }
        if (position.y + svgHeight >= rect.height || position.y <= 0) {
            dy *= -1; // Invertir dirección en Y
            playSound(svgElement.getAttribute('data-audio')); // Reproducir sonido al tocar el límite en Y
        }

        // Ajustar la posición del SVG suavemente al cambiar el tamaño de la ventana
        const maxX = rect.width - svgWidth;
        const maxY = rect.height - svgHeight;
        position.x = Math.max(0, Math.min(position.x, maxX));
        position.y = Math.max(0, Math.min(position.y, maxY));

        svgElement.style.transform = `translate(${position.x}px, ${position.y}px)`;
    };

    // Actualización del SVG cargado mediante botones
    document.querySelectorAll('button[data-svg]').forEach(button => {
        button.addEventListener('click', () => {
            const svgName = button.getAttribute('data-svg');
            const svgPath = `objetos/${svgName}.svg`;

            fetch(svgPath)
                .then(response => response.text())
                .then(svgData => {
                    svgElement.innerHTML = svgData;
                    svgElement.style.width = '100px'; // Restablecer tamaño predeterminado después de cargar el nuevo SVG
                    svgElement.style.height = '100px'; // Restablecer tamaño predeterminado después de cargar el nuevo SVG

                    // Actualizar el `data-audio` según el botón presionado
                    svgElement.setAttribute('data-audio', button.getAttribute('data-audio'));

                    // Ocultar el mensaje si hay un SVG
                    document.getElementById('no-svg-message').style.display = 'none';

                    // Habilitar el botón "Parar" solo si hay un SVG en pantalla
                    document.getElementById('parar').disabled = false;

                    // Llamar solo una vez para inicializar la posición aleatoria cuando se empieza el programa
                    if (!initialized) {
                        initializeRandomPosition();
                        initialized = true;
                    }
                })
                .catch(error => console.error('Error al cargar el SVG:', error));
        });
    });

    // Función para detener el movimiento del SVG
    const toggleMovement = () => {
        moving = !moving; // Alternar estado de movimiento
        document.getElementById('parar').textContent = moving ? 'Parar' : 'Continuar'; // Cambiar texto del botón
    };

    // Botón de Parar/Continuar
    document.getElementById('parar').addEventListener('click', toggleMovement);

    // Animación del movimiento
    setInterval(updatePosition, 16); // ~60 FPS
} else {
    console.error('El contenedor SVG no está presente en el DOM');
}

// Función para reproducir el sonido correspondiente al tipo de SVG
const playSound = (audioType) => {
    if (audioType) { // Asegurarse de que el audioType no esté vacío
        const audio = new Audio(`audios/${audioType}.wav`); // Ruta correcta con el tipo de sonido
        audio.play().catch(error => console.error('Error al reproducir el sonido:', error));
    }
};

// Función para inicializar la posición del SVG en un lugar aleatorio
const initializeRandomPosition = () => {
    const rect = container.getBoundingClientRect();
    const svgWidth = svgElement.offsetWidth;
    const svgHeight = svgElement.offsetHeight;

    position.x = Math.random() * (rect.width - svgWidth);
    position.y = Math.random() * (rect.height - svgHeight);

    svgElement.style.transform = `translate(${position.x}px, ${position.y}px)`;
};
