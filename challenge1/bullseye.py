import numpy as np
import matplotlib.pyplot as plt

# Define number of rings
num_rings = 5

# Define three different entry angles
entry_angles = [np.pi / 6, np.pi / 3, np.pi *1.5]  # 30°, 60°, and 90°

# Create figure
fig, ax = plt.subplots(figsize=(6, 6))
ax.set_xlim(-num_rings, num_rings)
ax.set_ylim(-num_rings, num_rings)
ax.set_aspect('equal')  # Ensure circles are not distorted

# Draw rings
for i in range(num_rings, 0, -1):
    circle = plt.Circle((0, 0), i, color='lightgray', ec='black', lw=1, fill=False)
    ax.add_patch(circle)

# Loop through different entry angles and plot paths
for theta in entry_angles:
    # Compute intersection point with the outermost ring
    radius = num_rings  # The rover reaches the outermost ring
    intersect_x = radius * np.cos(theta)
    intersect_y = radius * np.sin(theta)

    # Calculate the tangent angles (perpendicular to radius)
    tangent_angle1 = theta + np.pi / 2  # +90 degrees
    tangent_angle2 = theta - np.pi / 2  # -90 degrees

    # Define tangent line length
    tangent_length = radius * 0.5  # Adjust for visualization

    # Compute tangent line endpoints
    tangent_x1 = intersect_x + tangent_length * np.cos(tangent_angle1)
    tangent_y1 = intersect_y + tangent_length * np.sin(tangent_angle1)

    tangent_x2 = intersect_x + tangent_length * np.cos(tangent_angle2)
    tangent_y2 = intersect_y + tangent_length * np.sin(tangent_angle2)

    # Compute the perpendicular line through the intersection (collinear with the radius)
    perpendicular_x = [0, intersect_x]  # From the center to the intersection point
    perpendicular_y = [0, intersect_y]

    # Draw radial path (movement direction)
    ax.plot([num_rings, intersect_x], [0, intersect_y], linewidth=2, label=f"Path {np.degrees(theta):.0f}°")

    # Draw the tangent lines
    ax.plot([intersect_x, tangent_x1], [intersect_y, tangent_y1], color='grey', linestyle='dashed')
    ax.plot([intersect_x, tangent_x2], [intersect_y, tangent_y2], color='grey', linestyle='dashed')

    # Draw the perpendicular radius line
    ax.plot(perpendicular_x, perpendicular_y, color='black', linestyle='dotted', linewidth=2)

    # Mark intersection point
    ax.scatter(intersect_x, intersect_y, color='black', zorder=3)

# Remove axis labels and ticks
ax.set_xticks([])
ax.set_yticks([])
ax.set_frame_on(False)

# Set title and legend
plt.title("Tangent and Perpendicular Lines at Different Entry Angles", fontsize=14, pad=15)
ax.legend()

# Display the plot
plt.show()
