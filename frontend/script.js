
const sourceInput = document.getElementById("source");
const destinationInput = document.getElementById("destination");
const algorithmSelect = document.getElementById("algorithm");
const transportSelect = document.getElementById("transportMode");
const stopoverSelect = document.getElementById("stopover");
const budgetInput = document.getElementById("budget");
const routeOptions = document.getElementById("routeOptions");
const findRouteButton = document.getElementById("findRouteButton");
const resultMessage = document.getElementById("resultMessage");
const cityList = document.getElementById("cityList");

let cities = [];
let map;
let routeLayer;
let markerLayer;

// Initialize the map.
function initializeMap() {
    map = L.map("map").setView([27.5, 78.5], 5);

    L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
        attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors',
        maxZoom: 18
    }).addTo(map);

    routeLayer = L.layerGroup().addTo(map);
    markerLayer = L.layerGroup().addTo(map);
}

// Load available cities from the C++ backend.
async function loadCities() {
    try {
        const response = await fetch("/api/cities");

        if (!response.ok) {
            throw new Error("Could not load cities.");
        }

        cities = await response.json();

        cityList.innerHTML = "";
        stopoverSelect.innerHTML = '<option value="">No stopover</option>';

        cities.forEach(city => {
            const option = document.createElement("option");
            option.value = city.name;
            cityList.appendChild(option);

            const stopoverOption = document.createElement("option");
            stopoverOption.value = city.name;
            stopoverOption.textContent = city.name;
            stopoverSelect.appendChild(stopoverOption);
        });

    } catch (error) {
        resultMessage.textContent =
            "Unable to connect to the backend. Please start the C++ server.";
        resultMessage.className = "error-message";
    }
}

// Show or hide Dijkstra-specific options.
function updateAlgorithmOptions() {
    const isDijkstra = algorithmSelect.value === "dijkstra";

    routeOptions.style.display = isDijkstra ? "block" : "none";
}

algorithmSelect.addEventListener("change", updateAlgorithmOptions);

// Find city information using its name.
function getCity(name) {
    return cities.find(
        city => city.name.toLowerCase() === name.trim().toLowerCase()
    );
}

// Clear the previous route from the map.
function clearMap() {
    routeLayer.clearLayers();
    markerLayer.clearLayers();
}

// Display a route on the map.
function displayRoute(pathNames) {
    clearMap();

    const coordinates = [];

    pathNames.forEach(name => {
        const city = getCity(name);

        if (!city) return;

        const position = [city.latitude, city.longitude];
        coordinates.push(position);

        L.marker(position)
            .bindPopup(`<b>${city.name}</b>`)
            .addTo(markerLayer);
    });

    if (coordinates.length > 0) {
        const routeLine = L.polyline(coordinates, {
            color: "#2457a6",
            weight: 5
        });

        routeLine.addTo(routeLayer);
        map.fitBounds(routeLine.getBounds(), {
            padding: [40, 40]
        });
    }
}

// Display Dijkstra results.
function displayDijkstraResult(data) {
    resultMessage.innerHTML = "";

    if (!data.pathExists) {
        resultMessage.textContent = data.message || "No route exists.";
        resultMessage.className = "error-message";
        clearMap();
        return;
    }

    resultMessage.className = "";

    const heading = document.createElement("h4");
    heading.textContent = "Shortest Distance Route";
    resultMessage.appendChild(heading);

    const pathText = document.createElement("p");
    pathText.textContent = "Route: " + data.path.join(" → ");
    resultMessage.appendChild(pathText);

    const distance = document.createElement("p");
    distance.textContent =
        "Total Distance: " + Number(data.totalDistance).toFixed(2) + " km";
    resultMessage.appendChild(distance);

    const mode = document.createElement("p");
    mode.textContent =
        "Transport Mode: " + data.transportMode;
    resultMessage.appendChild(mode);

    const time = document.createElement("p");
    time.textContent =
        "Estimated Time: " +
        Number(data.estimatedTimeHours).toFixed(2) + " hours";
    resultMessage.appendChild(time);

    const cost = document.createElement("p");
    cost.textContent =
        "Estimated Cost: ₹" +
        Number(data.estimatedCost).toFixed(2);
    resultMessage.appendChild(cost);

    if (data.stopover) {
        const stopover = document.createElement("p");
        stopover.textContent = "Stopover: " + data.stopover;
        resultMessage.appendChild(stopover);
    }

    if (data.budget !== undefined) {
        const budget = document.createElement("p");
        budget.textContent = "Your Budget: ₹" +
            Number(data.budget).toFixed(2);
        resultMessage.appendChild(budget);

        const budgetStatus = document.createElement("p");
        budgetStatus.textContent = data.fitsBudget
            ? "Your estimated journey cost is within your budget."
            : "Your estimated journey cost exceeds your budget.";

        budgetStatus.className = data.fitsBudget
            ? "success-message"
            : "warning-message";

        resultMessage.appendChild(budgetStatus);
    }

    const note = document.createElement("p");
    note.className = "helper-text";
    note.textContent =
        "Time and cost are illustrative estimates, not live travel information.";
    resultMessage.appendChild(note);

    displayRoute(data.path);
}

// Display BFS or DFS traversal results.
function displayTraversalResult(data) {
    resultMessage.innerHTML = "";
    resultMessage.className = "";

    const heading = document.createElement("h4");
    heading.textContent = data.algorithm.toUpperCase() + " Traversal";
    resultMessage.appendChild(heading);

    const traversal = document.createElement("p");
    traversal.textContent =
        "Traversal Order: " + data.traversal.join(" → ");
    resultMessage.appendChild(traversal);

    const reached = document.createElement("p");
    reached.textContent = data.destinationReached
        ? "The destination is reachable from the starting city."
        : "The destination is not reachable from the starting city.";

    resultMessage.appendChild(reached);

    const note = document.createElement("p");
    note.className = "helper-text";
    note.textContent =
        "BFS and DFS show exploration order, not the shortest route.";
    resultMessage.appendChild(note);

    clearMap();
}

// Send the selected journey details to the backend.
async function findRoute() {
    const source = sourceInput.value.trim();
    const destination = destinationInput.value.trim();
    const algorithm = algorithmSelect.value;

    if (!source || !destination) {
        resultMessage.textContent =
            "Please enter both a starting city and a destination.";
        resultMessage.className = "error-message";
        return;
    }

    if (!getCity(source) || !getCity(destination)) {
        resultMessage.textContent =
            "Please select valid cities from the available city suggestions.";
        resultMessage.className = "error-message";
        return;
    }

    const requestData = {
        source: source,
        destination: destination,
        algorithm: algorithm
    };

    if (algorithm === "dijkstra") {
        requestData.transportMode = transportSelect.value;

        if (stopoverSelect.value) {
            requestData.stopover = stopoverSelect.value;
        }

        if (budgetInput.value.trim() !== "") {
            const budget = Number(budgetInput.value);

            if (!Number.isFinite(budget) || budget < 0) {
                resultMessage.textContent =
                    "Please enter a valid non-negative budget.";
                resultMessage.className = "error-message";
                return;
            }

            requestData.budget = budget;
        }
    }

    findRouteButton.disabled = true;
    findRouteButton.textContent = "Finding Route...";

    resultMessage.className = "";
    resultMessage.textContent = "Please wait while we calculate your journey.";

    try {
        const response = await fetch("/api/route", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(requestData)
        });

        const data = await response.json();

        if (!response.ok) {
            throw new Error(data.error || "Unable to calculate the route.");
        }

        if (algorithm === "dijkstra") {
            displayDijkstraResult(data);
        } else {
            displayTraversalResult(data);
        }

    } catch (error) {
        resultMessage.textContent = error.message;
        resultMessage.className = "error-message";
        clearMap();

    } finally {
        findRouteButton.disabled = false;
        findRouteButton.textContent = "Find Route";
    }
}

findRouteButton.addEventListener("click", findRoute);

// Start the application.
initializeMap();
loadCities();
updateAlgorithmOptions();
