import numpy as np
import matplotlib.pyplot as plt

# Define number of rings
num_rings = 5

# Define three different entry angles
entry_angles = [np.pi / 6, np.pi / 3, np.pi * 1.5]  # 30°, 60°, and 90°

# Define movement length for the slope line
move_length = num_rings * 0.5  # Adjust the length for visualization

# Define the specific contact point (4.33, 2.5) and slope (√3/2, 1/2)
contact_x, contact_y = 4.33, 2.5
slope_x, slope_y = np.sqrt(3)/2, 1/2  # Direction of the slope

# Compute endpoints for the slope line
slope_x1 = contact_x + move_length * slope_x
slope_y1 = contact_y + move_length * slope_y

slope_x2 = contact_x - move_length * slope_x
slope_y2 = contact_y - move_length * slope_y

# Create figure
fig, ax = plt.subplots(figsize=(6, 6))
ax.set_xlim(-num_rings, num_rings)
ax.set_ylim(-num_rings, num_rings)
ax.set_aspect('equal')  # Ensure circles are not distorted

# Draw rings
for i in range(num_rings, 0, -1):
    circle = plt.Circle((0, 0), i, color='lightgray', ec='black', lw=1, fill=False)
    ax.add_patch(circle)

# Store intersection coordinates
contact_points = []

# Loop through different entry angles and plot paths
for theta in entry_angles:
    # Compute intersection point with the outermost ring
    radius = num_rings  # The rover reaches the outermost ring
    intersect_x = radius * np.cos(theta)
    intersect_y = radius * np.sin(theta)

    # Store contact point
    contact_points.append((intersect_x, intersect_y))

    # Compute the perpendicular line through the intersection (collinear with the radius)
    perpendicular_x = [0, intersect_x]  # From the center to the intersection point
    perpendicular_y = [0, intersect_y]

    # Draw radial path (initial movement direction)
    ax.plot([num_rings, intersect_x], [0, intersect_y], linewidth=2, label=f"Path {np.degrees(theta):.0f}°")

    # Draw the perpendicular radius line
    ax.plot(perpendicular_x, perpendicular_y, color='black', linestyle='dotted', linewidth=2)

    # Mark intersection point
    ax.scatter(intersect_x, intersect_y, color='black', zorder=3)

# Draw the slope line at (4.33, 2.5) with slope (√3/2, 1/2)
ax.plot([slope_x1, slope_x2], [slope_y1, slope_y2], color='red', linestyle='dashed', linewidth=2, label="Slope (√3/2, 1/2)")

# Mark specific contact point (4.33, 2.5)
ax.scatter(contact_x, contact_y, color='blue', zorder=3, label="Special Contact Point (4.33, 2.5)")

# Remove axis labels and ticks
ax.set_xticks([])
ax.set_yticks([])
ax.set_frame_on(False)

# Set title and legend
plt.title("Slope Line at Contact Point (4.33, 2.5) with Slope (√3/2, 1/2)", fontsize=14, pad=15)
ax.legend()

# Display the plot
plt.show()

# Print contact points
print("Contact Points (Edge Coordinates):")
for i, (x, y) in enumerate(contact_points):
    print(f"Path {i+1}: (x: {x:.2f}, y: {y:.2f})")

# Print special slope point
print(f"Special Contact Point: (x: {contact_x}, y: {contact_y}) - Slope Line (√3/2, 1/2) Added")
